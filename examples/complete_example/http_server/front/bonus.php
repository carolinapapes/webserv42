#!/usr/bin/env php
<?php
/* ************************************************************************** */
/*                                                                            */
/*                                                       :::      ::::::::    */
/*   bonus.php                                         :+:      :+:    :+:    */
/*                                                   +:+ +:+         +:+      */
/*   By: capapes <capapes@student.42.fr>           +#+  +:+       +#+         */
/*                                               +#+#+#+#+#+   +#+            */
/*   Created: 2026/10/14 22:43:50 by capapes          #+#    #+#              */
/*   Updated: 2026/10/14 22:43:50 by capapes         ###   ########.fr        */
/*                                                                            */
/* ************************************************************************** */


$body = "Hello, world!\nCurrent time: " . date("Y-m-d H:i:s");

$response =
    "HTTP/1.1 200 OK\r\n" .
    "Content-Type: text/plain; charset=UTF-8\r\n" .
    "Content-Length: " . strlen($body) . "\r\n" .
    "Connection: close\r\n" .
    "\r\n" .
    $body;

echo $response;