/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: capapes <capapes@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 13:17:20 by capapes           #+#    #+#             */
/*   Updated: 2025/11/28 20:31:26 by capapes          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include <string>
#include "control_data.hpp"
#include "headers.hpp"

class Request {
public:
    Request();
    Request(const ControlData& cd, const Headers& h, const std::string& b);
    Request(const Request& other);
    Request& operator=(const Request& other);
    ~Request();

    void 				setControlData(const ControlData& cd);
    void 				setHeaders(const Headers& h);
    void 				setBody(const std::string& b);
    void 				setErrorCode(int code);

    const ControlData& 	getControlData() 	const;
    const Headers& 		getHeaders() 		const;
    const std::string& 	getBody() 			const;
    int 				getErrorCode() 		const;

private:
    ControlData 		controlData;
    Headers 			headers;
    std::string 		body;
    int 				errorCode;
};
