/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   request_parser.test.cpp                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: capapes <capapes@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/15 00:00:00 by capapes           #+#    #+#             */
/*   Updated: 2026/10/15 00:00:00 by capapes          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../support/http_request_test.hpp"
#include <request_parser.hpp>

static const std::string host = "Host: example.com\r\n";
static const std::string get = "GET / HTTP/1.1\r\n" + host + "\r\n";
static const std::string chunkHead = "POST /chunk HTTP/1.1\r\n" + host
    + "Transfer-Encoding: chunked\r\n\r\n";

static void outcome(TestSuite& suite, const RequestParser& parser,
                     RequestParseResult::Status expected, int code,
                     const std::string& name)
{
    suite.checkEqual(parser.result().status, expected, name + " status");
    suite.checkEqual(parser.result().getErrorCode(), code, name + " HTTP code");
    suite.checkEqual(parser.result().message.empty(), code == 0,
                     name + " diagnostic");
}

// =====================================================================
// 	ALL TWO-FRAGMENT SPLITS AND BYTEWISE INPUT
// =====================================================================
static void fragmented(TestSuite& suite, const std::string& wire,
                         const Request& expected)
{
    for (size_t split = 0; split <= wire.size(); ++split)
    {
        RequestParser parser;
        parser.feed(wire.substr(0, split));
        outcome(suite, parser, split == wire.size() ? RequestParseResult::COMPLETE
                    : RequestParseResult::INCOMPLETE, 0, "first fragment");
        parser.feed(wire.substr(split), true);
        outcome(suite, parser, RequestParseResult::COMPLETE, 0, "second fragment");
        checkRequest(suite, "fragmented fields", parser.result().request, expected);
        suite.check(parser.pendingData().empty(), "exact message has no surplus");
    }
    RequestParser bytewise;
    for (size_t i = 0; i < wire.size(); ++i)
    {
        bytewise.feed(wire.substr(i, 1));
        suite.checkEqual(bytewise.result().status, i + 1 == wire.size()
            ? RequestParseResult::COMPLETE : RequestParseResult::INCOMPLETE,
            "bytewise input does not dispatch early");
    }
    checkRequest(suite, "bytewise fields", bytewise.result().request, expected);
}

static void fixedBodies(TestSuite& suite)
{
    Headers headers;
    headers.add("Host", "example.com");
    headers.add("Content-Length", "6");
    const std::string body("a\0b\r\nc", 6);
    const std::string head = "POST /binary HTTP/1.1\r\n" + host
        + "Content-Length: 6\r\n\r\n";
    fragmented(suite, head + body,
        Request(ControlData("POST", "/binary", "HTTP/1.1"), headers, body));
    const RequestParseResult partial = parseRequest(head + body.substr(0, 5));
    suite.checkEqual(partial.status, RequestParseResult::INCOMPLETE,
                     "short fixed body remains incomplete");
    suite.checkEqual(partial.getErrorCode(), 0, "short body is not premature 400");
    suite.checkEqual(validateRequest(head + body.substr(0, 5)).getErrorCode(), 400,
                     "final truncated fixed body is rejected");
    suite.checkEqual(parseRequest(head + body + get, true).request.getBody(), body,
                     "whole-buffer wrapper respects exact body boundary");

    headers.remove("Content-Length");
    fragmented(suite, get,
        Request(ControlData("GET", "/", "HTTP/1.1"), headers, ""));
    suite.checkEqual(parseRequest("POST / HTTP/1.1\r\n" + host + "\r\n").status,
                     RequestParseResult::COMPLETE, "unframed request has zero body");
    suite.checkEqual(validateRequest("GET / HTTP/1.1\r\n" + host
        + "Content-Length: 3\r\n\r\nabc").getBody(), std::string("abc"),
        "framing is independent of method semantics");
    RequestParser zero;
    zero.feed("POST / HTTP/1.1\r\n" + host + "Content-Length: 0\r\n\r\n" + get);
    suite.checkEqual(zero.result().request.getBody(), std::string(), "zero-length body");
    suite.checkEqual(zero.pendingData(), get, "zero-length body preserves next message");
}

static void chunkedBodies(TestSuite& suite)
{
    Headers headers;
    headers.add("Host", "example.com");
    headers.add("Transfer-Encoding", "chunked");
    const std::string chunks = std::string("2;ext=token\r\n") + std::string("a\0", 2) + "\r\n"
        + "4 ; sig=\"a\\\"b\";flag\r\nb\r\nc\r\n"
        + "000;done\r\nX-Checksum: ok\r\nx-checksum: two\r\n\r\n";
    const std::string body("a\0b\r\nc", 6);
    fragmented(suite, chunkHead + chunks,
        Request(ControlData("POST", "/chunk", "HTTP/1.1"), headers, body));
    const Request decoded = validateRequest(chunkHead + chunks);
    suite.check(!decoded.getHeaders().has("X-Checksum"), "trailers are never merged");
    suite.check(decoded.getHeaders().has("Transfer-Encoding"), "original headers retained");
    suite.checkEqual(validateRequest(chunkHead + "A\r\n0123456789\r\n0\r\n\r\n").getBody(),
                     std::string("0123456789"), "uppercase hexadecimal chunk size");
    suite.checkEqual(validateRequest(chunkHead + "0\r\n\r\n").getBody(),
                     std::string(), "empty chunked message decodes to empty body");
    suite.checkEqual(parseRequest(chunkHead + "0\r\n").status,
                     RequestParseResult::INCOMPLETE, "zero chunk still needs trailer terminator");
    const std::string extension = std::string("1;unknown=\"")
        + static_cast<char>(128) + "\"\r\nx\r\n0\r\n\r\n";
    suite.checkEqual(validateRequest(chunkHead + extension).getBody(),
                     std::string("x"), "opaque bytes in an ignored quoted extension");
}

// =====================================================================
// 	PIPELINING, INDEPENDENT INSTANCES AND RESET
// =====================================================================
static void pipeline(TestSuite& suite)
{
    const std::string fixed = "POST /fixed HTTP/1.1\r\n" + host
        + "Content-Length: 3\r\n\r\nxyz";
    const std::string chunked = chunkHead + "1\r\nq\r\n0\r\n\r\n";
    RequestParser parser;
    parser.feed(get + fixed + chunked + "GET /fourth HTTP/1.1\r");
    outcome(suite, parser, RequestParseResult::COMPLETE, 0, "pipeline first");
    const RequestParseResult snapshot = parser.result();
    suite.checkEqual(parser.pendingData(), fixed + chunked + "GET /fourth HTTP/1.1\r",
                     "pipelined bytes retained exactly");
    parser.next();
    suite.checkEqual(parser.result().request.getBody(), std::string("xyz"), "pipeline fixed body");
    parser.next();
    suite.checkEqual(parser.result().request.getBody(), std::string("q"), "pipeline chunked body");
    parser.next();
    outcome(suite, parser, RequestParseResult::INCOMPLETE, 0, "pipeline partial fourth");
    parser.feed("\n" + host + "\r\n", true);
    outcome(suite, parser, RequestParseResult::COMPLETE, 0, "pipeline completed fourth");
    suite.checkEqual(parser.result().request.getControlData().requestTarget,
                     std::string("/fourth"), "fourth request has fresh fields");
    suite.checkEqual(snapshot.request.getControlData().requestTarget,
                     std::string("/"), "completed snapshot owns its data");
    parser.next();
    outcome(suite, parser, RequestParseResult::INCOMPLETE, 0, "drained EOF pipeline");

    parser.reset();
    parser.feed(get + fixed.substr(0, fixed.size() - 1), true);
    parser.next();
    outcome(suite, parser, RequestParseResult::ERROR, 400, "EOF applies to buffered next request");
    parser.feed(get);
    outcome(suite, parser, RequestParseResult::ERROR, 400, "error remains sticky");
    suite.check(parser.pendingData().empty(), "error discards untrusted pending bytes");
    parser.reset();
    parser.feed(get);
    outcome(suite, parser, RequestParseResult::COMPLETE, 0, "reset after failure");
    parser.feed(fixed);
    suite.checkEqual(parser.result().request.getControlData().method, std::string("GET"),
                     "feeding a complete parser preserves its current result");
    parser.next();
    suite.checkEqual(parser.result().request.getBody(), std::string("xyz"), "feed while complete queues input");

    RequestParser first;
    RequestParser second;
    first.feed(fixed.substr(0, fixed.size() - 1));
    second.feed(chunkHead + "1\r\na");
    first.feed("z");
    outcome(suite, first, RequestParseResult::COMPLETE, 0, "first independent connection");
    outcome(suite, second, RequestParseResult::INCOMPLETE, 0, "second independent connection");
    second.feed("\r\n0\r\n\r\n");
    suite.checkEqual(second.result().request.getBody(), std::string("a"), "interleaved chunk body");
    suite.checkEqual(first.result().request.getBody(), std::string("xyz"), "other connection unaffected");
    first.reset();
    outcome(suite, first, RequestParseResult::INCOMPLETE, 0, "explicit reset");
    suite.check(first.result().request.getHeaders().begin()
                == first.result().request.getHeaders().end(), "reset clears headers");
    parser.reset();
    parser.feed(get + chunkHead + "bad-size\r\n" + get);
    parser.next();
    outcome(suite, parser, RequestParseResult::ERROR, 400, "malformed pipelined message");
    parser.next();
    outcome(suite, parser, RequestParseResult::ERROR, 400, "no resynchronization after framing error");
    suite.check(parser.pendingData().empty(), "error discards later pipelined requests");
}

// =====================================================================
// 	MALFORMED CHUNK FRAMING AND END OF INPUT
// =====================================================================
static void invalidChunks(TestSuite& suite)
{
    const char* invalid[] = {
        "\r\n", "Z\r\n", "+1\r\n", "-1\r\n", " 1\r\n", "0x1\r\n", "1.0\r\n",
        "1;\r\n", "1;x=\r\n", "1;=x\r\n", "1;bad(name=x\r\n",
        "1;x=\"unterminated\r\n", "1;x=\"bad\001value\"\r\n", "1 \r\n",
        "1\r\naX", "1\r\na\rX", "1\na\r\n", "1\r\na\n",
        "0\r\nHost: other.example\r\n\r\n",
        "0\r\ncOnTeNt-LeNgTh: 0\r\n\r\n",
        "0\r\nTransfer-Encoding: chunked\r\n\r\n",
        "0\r\nAuthorization: ignored\r\n\r\n",
        "0\r\nBad Name: x\r\n\r\n", "0\r\n folded\r\n\r\n",
        "0\r\nMissingColon\r\n\r\n", "0\r\nX: x\n"
    };
    for (size_t i = 0; i < sizeof(invalid) / sizeof(invalid[0]); ++i)
    {
        const std::string wire = chunkHead + invalid[i];
        for (size_t split = 0; split <= wire.size(); ++split)
        {
            RequestParser parser;
            parser.feed(wire.substr(0, split));
            parser.feed(wire.substr(split), true);
            outcome(suite, parser, RequestParseResult::ERROR, 400, "malformed fragmented chunk");
        }
    }
    const std::string valid = chunkHead + "2\r\nab\r\n0\r\nX-End: yes\r\n\r\n";
    for (size_t size = chunkHead.size(); size < valid.size(); ++size)
    {
        RequestParser parser;
        parser.feed(valid.substr(0, size));
        outcome(suite, parser, RequestParseResult::INCOMPLETE, 0, "valid unfinished chunked prefix");
        parser.feed("", true);
        outcome(suite, parser, RequestParseResult::ERROR, 400, "truncated chunked message at EOF");
    }
    suite.checkEqual(validateRequest(chunkHead + "FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF\r\n").getErrorCode(),
                     413, "hexadecimal overflow is detected before multiplication");
    suite.checkEqual(validateRequest(chunkHead + "0\r\nX: a" + std::string(1, '\0')
        + "b\r\n\r\n").getErrorCode(), 400, "NUL in a trailer is rejected");
}

// =====================================================================
// 	CONFIGURABLE BOUNDS AND EXACT LIMITS
// =====================================================================
static void bounded(TestSuite& suite, const std::string& wire,
                      const RequestParserLimits& limits, int code,
                      const std::string& name)
{
    RequestParser parser(limits);
    parser.feed(wire, true);
    outcome(suite, parser, code ? RequestParseResult::ERROR
                : RequestParseResult::COMPLETE, code, name);
}

static void limits(TestSuite& suite)
{
    RequestParserLimits limits;
    limits.maxRequestLineBytes = std::string("GET / HTTP/1.1\r\n").size();
    bounded(suite, get, limits, 0, "exact request line limit");
    --limits.maxRequestLineBytes;
    bounded(suite, get, limits, 414, "request line exceeds limit");
    limits = RequestParserLimits();
    limits.maxHeaderBytes = host.size() + 2;
    bounded(suite, get, limits, 0, "exact aggregate header limit");
    --limits.maxHeaderBytes;
    bounded(suite, get, limits, 431, "aggregate header limit");
    limits = RequestParserLimits();
    limits.maxHeaderBytes = 40;
    bounded(suite, "GET / HTTP/1.1\r\n" + host + "X: a\r\nX: b\r\nX: c\r\nX: d\r\n\r\n",
             limits, 431, "many small headers exceed aggregate bound");

    const std::string fixed = "POST / HTTP/1.1\r\n" + host + "Content-Length: ";
    limits = RequestParserLimits();
    limits.maxBodyBytes = 3;
    bounded(suite, fixed + "3\r\n\r\nabc", limits, 0, "exact configured body limit");
    bounded(suite, fixed + "4\r\n\r\n", limits, 413, "declared body rejected before arrival");
    bounded(suite, chunkHead + "2\r\nab\r\n1\r\nc\r\n0\r\n\r\n", limits, 0, "exact decoded body bound");
    bounded(suite, chunkHead + "2\r\nab\r\n2\r\n", limits, 413, "cumulative decoded body bound");
    limits.maxBodyBytes = 0;
    bounded(suite, fixed + "0\r\n\r\n", limits, 0, "zero limit permits empty body");
    bounded(suite, chunkHead + "0\r\n\r\n", limits, 0, "zero limit permits empty chunks");
    bounded(suite, chunkHead + "1\r\n", limits, 413, "zero body limit rejects data chunk");

    limits = RequestParserLimits();
    limits.maxChunkMetadataBytes = 8;
    bounded(suite, chunkHead + "1\r\na\r\n0\r\n\r\n", limits, 0, "exact chunk metadata bound");
    --limits.maxChunkMetadataBytes;
    bounded(suite, chunkHead + "1\r\na\r\n0\r\n\r\n", limits, 413, "aggregate chunk metadata bound");
    limits = RequestParserLimits();
    limits.maxChunkLineBytes = 5;
    bounded(suite, chunkHead + "0;x=y\r\n\r\n", limits, 413, "chunk extension line bound");
    limits = RequestParserLimits();
    limits.maxHeaderBytes = host.size() + std::string("Transfer-Encoding: chunked\r\n").size()
        + 2 + std::string("X: yes\r\n\r\n").size();
    bounded(suite, chunkHead + "0\r\nX: yes\r\n\r\n", limits, 0, "exact combined header/trailer bound");
    --limits.maxHeaderBytes;
    bounded(suite, chunkHead + "0\r\nX: yes\r\n\r\n", limits, 431, "trailer aggregate bound");

    limits = RequestParserLimits();
    limits.maxPendingBytes = get.size();
    bounded(suite, get, limits, 0, "exact pending-input bound");
    bounded(suite, get + "x", limits, 413, "oversized single feed is rejected before buffering");
    RequestParser pending(limits);
    pending.feed(get);
    pending.feed(get);
    pending.feed("x");
    outcome(suite, pending, RequestParseResult::ERROR, 413, "queued pipeline bound");
    suite.check(pending.pendingData().empty(), "buffer-limit error releases pending data");
    pending.reset();
    pending.feed(get);
    outcome(suite, pending, RequestParseResult::COMPLETE, 0, "reset retains configured limits");

    limits = RequestParserLimits();
    limits.maxPendingBytes = 8;
    RequestParser streaming(limits);
    const std::string wire = fixed + "3\r\n\r\nabc";
    for (size_t i = 0; i < wire.size(); ++i)
    {
        streaming.feed(wire.substr(i, 1));
        if (streaming.result().status == RequestParseResult::ERROR)
            break;
    }
    // Pending bytes include an unfinished line; a line longer than eight bytes
    // is intentionally rejected, even when fragments arrive one byte at a time.
    outcome(suite, streaming, RequestParseResult::ERROR, 413, "unfinished-line buffer bound");

    limits.maxPendingBytes = 128;
    limits.maxBodyBytes = 512;
    std::string binary;
    for (size_t i = 0; i < 512; ++i)
        binary += static_cast<char>(i % 256);
    const std::string large = fixed + "512\r\n\r\n" + binary;
    RequestParser largeBody(limits);
    for (size_t pos = 0; pos < large.size(); pos += 32)
    {
        largeBody.feed(large.substr(pos, 32));
        suite.check(largeBody.pendingData().size() <= limits.maxPendingBytes,
                     "streamed body does not accumulate in pending input");
    }
    outcome(suite, largeBody, RequestParseResult::COMPLETE, 0, "body larger than transport buffer");
    suite.checkEqual(largeBody.result().request.getBody(), binary,
                     "all 256 octet values survive streamed framing");
    bounded(suite, large, limits, 413, "single feed still obeys pending-input ceiling");
}

int main()
{
    TestSuite suite("Incremental HTTP framing");
    try
    {
        fixedBodies(suite);
        chunkedBodies(suite);
        pipeline(suite);
        invalidChunks(suite);
        limits(suite);
    }
    catch (const std::exception& error)
    {
        suite.check(false, std::string("unexpected exception: ") + error.what());
    }
    return suite.finish();
}
