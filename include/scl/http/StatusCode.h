#pragma once
#include <string>
#include <scl/macros.h>
#include <scl/http/Version.h>
#include <scl/utils/Optional.h>
#include <scl/alias/aliases.h>

namespace scl{
	namespace http{
		class StatusCode{
			public:
				using name_type = stringLiteral;
				using status_type = scl::alias::uint;
				using message_type = stringLiteral;
				using version_type = Version;

			protected:
				const name_type shortInfo;
				const status_type code;
				const message_type msg{};
				const version_type& vers;

				constexpr StatusCode(name_type name, status_type status, message_type message, const version_type& version = Version::V1_1)
				: shortInfo{name}, code{status}, msg{message}, vers{version} {
				}

			public:
				//friend class scl::utils::ToString<StatusCode>;

				constexpr name_type name() const{
					return this->shortInfo;
				}

				constexpr status_type status() const{
					return this->code;
				}

				constexpr message_type message() const{
					return this->msg;
				}

				constexpr version_type version() const{
					return this->vers;
				}

			public:
				constexpr explicit operator status_type() const{
					return this->code;
				}

				constexpr bool operator==(const StatusCode& other) const{
					return this->code == other.code;
				}

				constexpr bool operator!=(const StatusCode& other) const{
					return !(*this == other);
				}

				/**
				 * Determine whether or not this status code is informational
				 * @return TRUE if it is, FALSE otherwise
				 */
				constexpr bool isInformational() const{
					return 100 <= static_cast<status_type>(*this)
					&& static_cast<status_type>(*this) < 200;
				}

				/**
				 * Determine whether or not this status code is for a successful request
				 * @return TRUE if it is, FALSE otherwise
				 */
				constexpr bool isSuccessful() const{
					return 200 <= static_cast<status_type>(*this)
					&& static_cast<status_type>(*this) < 300;
				}

				/**
				 * Determine whether or not this status code is one of a redirection
				 * @return TRUE if it is, FALSE otherwise
				 */
				constexpr bool isRedirect() const{
					return 300 <= static_cast<status_type>(*this)
					&& static_cast<status_type>(*this) < 400;
				}

				/**
				 * Determine whether or not this status code is one of a client error
				 * @return TRUE if it is, FALSE otherwise
				 */
				constexpr bool isClientError() const{
					return 400 <= static_cast<status_type>(*this)
					&& static_cast<status_type>(*this) < 500;
				}

				/**
				 * Determine whether or not this status code is one of a server error
				 * @return TRUE if it is, FALSE otherwise
				 */
				constexpr bool isServerError() const{
					return 500 <= static_cast<status_type>(*this)
					&& static_cast<status_type>(*this) < 600;
				}

				/**
				 * Determine whether or not this status code indicates that the requests was fulfilled
				 * @return TRUE if it is, FALSE otherwise
				 */
				constexpr bool isOk() const{
					return (this->isInformational() || this->isSuccessful() ||this->isRedirect())
						   && *this != SEE_OTHER //did not move
					&& *this != USE_PROXY //returns asking for proxy
					/*&& *this != UNUSED //unused status code*/
					&& *this != TEMPORARY_REDIRECT; //did not move
				}

			public:
#define CODE(name) static const StatusCode name
				//1xy - Informational
				CODE(CONTINUE); //RFC 7231 6.2.1
				CODE(SWITCHING_PROTOCOLS); //RFC 7231 6.2.2
				CODE(PROCESSING); //RFC 2518 10.1
                CODE(EARLY_HINTS); //RFC 8297 2

				//2xy - Successful
				CODE(OK);  //RFC 7231 6.3.1
				CODE(CREATED); //RFC 7231 6.3.2
				CODE(ACCEPTED); //RFC 7231 6.3.3
				CODE(NON_AUTHORITATIVE_INFORMATION); //RFC 7231 6.3.4
				CODE(NO_CONTENT); //RFC 7231 6.3.5
				CODE(RESET_CONTENT); //RFC 7231 6.3.6
				CODE(PARTIAL_CONTENT); //RFC 7233 4.1
				CODE(MULTI_STATUS); //RFC 4918 11.1
				CODE(ALREADY_REPORTED); //RFC 5842 7.1
				CODE(IM_USED); //RFC 3229 10.4.1

				//3xy - Redirections
				CODE(MULTIPLE_CHOICES); //RFC 7231 6.4.1
				CODE(MOVED_PERMANENTLY); //RFC 7231 6.4.2
				CODE(FOUND); //RFC 7231 6.4.3
				CODE(SEE_OTHER); //RFC 7231 6.4.4
				CODE(NOT_MODIFIED); //RFC 7232 4.1
				/*CODE(UNUSED);*/
				CODE(USE_PROXY); //RFC 7231 6.4.5
				CODE(TEMPORARY_REDIRECT); //RFC 7231 6.4.7
				CODE(PERMANENT_REDIRECT); //RFC 7238 3

				//4xy - Client Error
				CODE(BAD_REQUEST); //RFC 7231 6.5.1
				CODE(UNAUTHORIZED); //RFC 7235 3.1
				CODE(PAYMENT_REQUIRED); //RFC 7231 6.5.2
				CODE(FORBIDDEN); //RFC 7231 6.5.3
				CODE(NOT_FOUND); //RFC 7231 6.5.4
				CODE(METHOD_NOT_ALLOWED); //RFC 7231 6.5.5
				CODE(NOT_ACCEPTABLE); //RFC 7231 6.5.6
				CODE(PROXY_AUTHENTICATION_REQUIRED); //RFC 7235 3.2
				CODE(REQUEST_TIMEOUT); //RFC 7231 6.5.7
				CODE(CONFLICT); //RFC 7231 6.5.8
				CODE(GONE); //RFC 7231 6.5.9
				CODE(LENGTH_REQUIRED); //RFC 7231 6.5.10
				CODE(PRECONDITION_FAILED); //RFC 7232 4.2
				CODE(REQUEST_ENTITY_TOO_LARGE); //RFC 7231 6.5.11  //TODO: Change name
				CODE(REQUEST_URI_TOO_LONG); //RFC 7231 6.5.12 //TODO: Change name
				CODE(UNSUPPORTED_MEDIA_TYPE); //RFC 7231 6.5.13
				CODE(REQUESTED_RANGE_NOT_SATISFIABLE); //RFC 7233 4.4
				CODE(EXPECTATION_FAILED); //RFC 7231 6.5.14
				CODE(TEAPOT); //RFC 7168 2.3.3
				CODE(MISDIRECTED_REQUEST); // RFC 7540 9.1.2
				CODE(UNPROCESSABLE_ENTITY); //RFC 4918 11.2
				CODE(LOCKED); //RFC 4918 11.3
				CODE(FAILED_DEPENDENCY); //RFC 4918 11.4
				CODE(TOO_EARLY); //RFC 8470 5.2 //TODO: add to node/deno
				CODE(UPGRADE_REQUIRED); //RFC 7231 6.5.15
				CODE(PRECONDITION_REQUIRED); //RFC 6585 3
				CODE(TOO_MANY_REQUESTS); //RFC 6585 4
				CODE(REQUEST_HEADER_FIELDS_TOO_LARGE); //RFC 6585 5
				CODE(UNAVAILABLE_FOR_LEGAL_REASONS); //RFC 7725 3

				//5xy - Server Error
				CODE(INTERNAL_SERVER_ERROR); //RFC 7231 6.6.1
				CODE(NOT_IMPLEMENTED); //RFC 7231 6.6.2
				CODE(BAD_GATEWAY); //RFC 7231 6.6.3
				CODE(SERVICE_UNAVAILABLE); //RFC 7231 6.6.4
				CODE(GATEWAY_TIMEOUT); //RFC 7231 6.6.5
				CODE(HTTP_VERSION_NOT_SUPPORTED); //RFC 7231 6.6.6
				CODE(VARIANT_ALSO_NEGOTIATES); //RFC 2295 8.1
				CODE(INSUFFICIENT_STORAGE); //RFC 4918 11.5
				CODE(LOOP_DETECTED); //RFC 5842 7.2
                CODE(BANDWIDTH_LIMIT_EXCEEDED); //no RFC
				CODE(NOT_EXTENDED); //RFC 2774 7
				CODE(NETWORK_AUTHENTICATION_REQUIRED); //RFC 6585 6
#undef CODE

			public:
				/**
				 * Get an optional StatusCode from the status code
				 * @param status being the status code to construct from
				 * @return an empty optional if the status is invalid, the correct instance otherwise
				 *
				 * @warning Uses a very long switch statement
				 */
				static scl::utils::Optional<StatusCode> fromCode(const status_type& status){
					/*if(
						status < 100
						|| (103 < status && status < 200)
						|| (226 < status && status < 300)
						|| (308 < status && status < 400)
						|| (451 < status && status < 500)
						|| 511 < status
					)//if invalid status
						return scl::utils::none;*/

					switch(status){
						case 100: return CONTINUE;
						case 101: return SWITCHING_PROTOCOLS;
						case 102: return PROCESSING;
                        case 103: return EARLY_HINTS;

						case 200: return OK;
						case 201: return CREATED;
						case 202: return ACCEPTED;
						case 203: return NON_AUTHORITATIVE_INFORMATION;
						case 204: return NO_CONTENT;
						case 205: return RESET_CONTENT;
						case 206: return PARTIAL_CONTENT;
						case 207: return MULTI_STATUS;
						case 208: return ALREADY_REPORTED;
						case 226: return IM_USED;

						case 300: return MULTIPLE_CHOICES;
						case 301: return MOVED_PERMANENTLY;
						case 302: return FOUND;
						case 303: return SEE_OTHER;
						case 304: return NOT_MODIFIED;
						case 305: return USE_PROXY;
						case 307: return TEMPORARY_REDIRECT;
						case 308: return PERMANENT_REDIRECT;

						case 400: return BAD_REQUEST;
						case 401: return UNAUTHORIZED;
						case 402: return PAYMENT_REQUIRED; //only reserved but make it available
						case 403: return FORBIDDEN;
						case 404: return NOT_FOUND;
						case 405: return METHOD_NOT_ALLOWED;
						case 406: return NOT_ACCEPTABLE;
						case 407: return PROXY_AUTHENTICATION_REQUIRED;
						case 408: return REQUEST_TIMEOUT;
						case 409: return CONFLICT;
						case 410: return GONE;
						case 411: return LENGTH_REQUIRED;
						case 412: return PRECONDITION_FAILED;
						case 413: return REQUEST_ENTITY_TOO_LARGE;
						case 414: return REQUEST_URI_TOO_LONG;
						case 415: return UNSUPPORTED_MEDIA_TYPE;
						case 416: return REQUESTED_RANGE_NOT_SATISFIABLE;
						case 417: return EXPECTATION_FAILED;
						case 418: return TEAPOT;
						case 421: return MISDIRECTED_REQUEST;
						case 422: return UNPROCESSABLE_ENTITY;
						case 423: return LOCKED;
						case 424: return FAILED_DEPENDENCY;
						case 425: return TOO_EARLY;
						case 426: return UPGRADE_REQUIRED;
						case 428: return PRECONDITION_REQUIRED;
						case 429: return TOO_MANY_REQUESTS;
						case 431: return REQUEST_HEADER_FIELDS_TOO_LARGE;
						case 451: return UNAVAILABLE_FOR_LEGAL_REASONS;

						case 500: return INTERNAL_SERVER_ERROR;
						case 501: return NOT_IMPLEMENTED;
						case 502: return BAD_GATEWAY;
						case 503: return SERVICE_UNAVAILABLE;
						case 504: return GATEWAY_TIMEOUT;
						case 505: return HTTP_VERSION_NOT_SUPPORTED;
						case 506: return VARIANT_ALSO_NEGOTIATES;
						case 507: return INSUFFICIENT_STORAGE;
						case 508: return LOOP_DETECTED;
                        case 509: return BANDWIDTH_LIMIT_EXCEEDED;
						case 510: return NOT_EXTENDED;
						case 511: return NETWORK_AUTHENTICATION_REQUIRED;

						default: return scl::utils::none;
					}
				};
		};

#define DEF_CODE(name, shortInfo, code, msg) \
		/** \
		 * msg \
		 * @var const StatusCode StatusCode::name
		 * @brief shortInfo
		 * @details msg
		 */ \
		const StatusCode StatusCode::name{shortInfo, code, msg};

		//1xy
		DEF_CODE(CONTINUE, "Continue", 100, "Continue with the request");
		DEF_CODE(SWITCHING_PROTOCOLS, "Switching Protocols", 101, "Switching protocols");
		DEF_CODE(PROCESSING, "Processing", 102, "The server has accepted the request but has not yet completed it");
		DEF_CODE(EARLY_HINTS, "Early Hints", 103, "The server is likely to send a final response with the header fields included in the informational response");

		//2xy
		DEF_CODE(OK, "Ok", 200, "Request fulfilled");
		DEF_CODE(CREATED, "Created", 201, "Resource has been created");
		DEF_CODE(ACCEPTED, "Accepted", 202, "Request accepted for processing");
		DEF_CODE(NON_AUTHORITATIVE_INFORMATION, "Non-Authoritative Information", 203, "Request fulfilled but has been modified");
		DEF_CODE(NO_CONTENT, "No Content", 204, "Request fulfilled, no entity-body returned");
		DEF_CODE(RESET_CONTENT, "Reset Content", 205, "Request fulfilled, user agent should reset the document view");
		DEF_CODE(PARTIAL_CONTENT, "Partial Content", 206, "Partial GET request fulfilled");
		DEF_CODE(MULTI_STATUS, "Multi Status", 207, "Statuses for multiple operations");
		DEF_CODE(ALREADY_REPORTED, "Already reported", 208, "One of the requested resources has already been reported");
		DEF_CODE(IM_USED, "IM Used", 226, "The server has fulfilled a GET request for the resource, and the response is a representation of the result of one or more instance-manipulation applied to the current instance");

		//3xy
		DEF_CODE(MULTIPLE_CHOICES, "Multiple Choices", 300, "The requested resource corresponds to any one of a set of representations");
		DEF_CODE(MOVED_PERMANENTLY, "Moved Permanently", 301, "The requested resource has been assigned a new permanent URI");
		DEF_CODE(FOUND, "Found", 302, "The requested resource has been delivered but resides temporarily under a different URI");
		DEF_CODE(SEE_OTHER, "See Other", 303, "The requested resource can be found under a different URI");
		DEF_CODE(NOT_MODIFIED, "Not Modified", 304, "Document has not been modified");
		DEF_CODE(USE_PROXY, "Use Proxy", 305, "The requested resource must be accessed through the given proxy");
		/*DEF_CODE(UNUSED, "[[Unused]]", 306, "[[Was used in previous version of the specification, no longer used, reserved]]");*/
		DEF_CODE(TEMPORARY_REDIRECT, "Temporary Redirect", 307, "The requested resource resides temporarily under a different URI");
		DEF_CODE(PERMANENT_REDIRECT, "Permanent Redirect", 308, "The requested resource resides permanently under a different URI");

		//4xy
		DEF_CODE(BAD_REQUEST, "Bad Request", 400, "Request could not be understood due to malformed syntax");
		DEF_CODE(UNAUTHORIZED, "Unauthorized", 401, "Request requires user authentication");
		DEF_CODE(PAYMENT_REQUIRED, "Payment Required", 402, "[[This code is reserved for future use]]");
		DEF_CODE(FORBIDDEN, "Forbidden", 403, "Request understood but the server refuses to fulfill it");
		DEF_CODE(NOT_FOUND, "Not Found", 404, "The requested resource has not been found");
		DEF_CODE(METHOD_NOT_ALLOWED, "Method Not Allowed", 405, "The specified method is not allowed for the identified resource");
		DEF_CODE(NOT_ACCEPTABLE, "Not Acceptable", 406, "Responses would not be acceptable according to accept headers in the request");
		DEF_CODE(PROXY_AUTHENTICATION_REQUIRED, "Proxy Authentication Required", 407, "Client must first authenticate itself with the proxy");
		DEF_CODE(REQUEST_TIMEOUT, "Request Timeout", 408, "Client did not produce a request within the time the server was prepared to wait");
		DEF_CODE(CONFLICT, "Conflict", 409, "Request could not be completed due to a conflict with the current state of the resource");
		DEF_CODE(GONE, "Gone", 410, "The requested resource is no longer available");
		DEF_CODE(LENGTH_REQUIRED, "Length Required", 411, "Content-Length header missing");
		DEF_CODE(PRECONDITION_FAILED, "Precondition Failed", 412, "Precondition given in one or more request-header fields evaluated to false");
		DEF_CODE(REQUEST_ENTITY_TOO_LARGE, "Request Entity Too Large", 413, "Provided request entity is larger than the server is willing/able to process");
		DEF_CODE(REQUEST_URI_TOO_LONG, "Request-URI Too Long", 414, "Request-URI is longer than the server is willing to process");
		DEF_CODE(UNSUPPORTED_MEDIA_TYPE, "Unsupported Media Type", 415, "The format of the entity of the request is not supported");
		DEF_CODE(REQUESTED_RANGE_NOT_SATISFIABLE, "Requested Range Not Satisfiable", 416, "Request Range could not be satisfied");
		DEF_CODE(EXPECTATION_FAILED, "Expectation Failed", 417, "Request Expect could not be met");
		DEF_CODE(TEAPOT, "I'm a teapot", 418, "I'm a teapot");
		DEF_CODE(MISDIRECTED_REQUEST, "Misdirected Request", 421, "Request was directed at a server that is not able to produce a response");
		DEF_CODE(UNPROCESSABLE_ENTITY, "Unprocessable Entity", 422, "The server understands the content type and the syntax is correct but was unable to process the contained instructions");
		DEF_CODE(LOCKED, "Locked", 423, "The requested resource is locked");
		DEF_CODE(FAILED_DEPENDENCY, "Failed Dependency", 424, "The server could not process the requested action because it depends on an action that failed");
		DEF_CODE(TOO_EARLY, "Too Early", 425, "The server is unwilling to risk processing a request that might be replayed");
		DEF_CODE(UPGRADE_REQUIRED, "Upgrade Required", 426, "The server refuses to perform the request using the current protocol but might if the client upgrades to a different protocol");
		DEF_CODE(PRECONDITION_REQUIRED, "Precondition Required", 428, "The server requires the request to be conditional (missing If-Match header)");
		DEF_CODE(TOO_MANY_REQUESTS, "Too Many Requests", 429, "The user has sent too many requests in a given amount of time");
		DEF_CODE(REQUEST_HEADER_FIELDS_TOO_LARGE, "Request Header Fields Too Large", 431, "The server is unwilling to process the request because its header fields are too large");
		DEF_CODE(UNAVAILABLE_FOR_LEGAL_REASONS, "Unavailable For Legal Reasons", 451, "The requested resource is unavailable for legal reasons");

		//5xy
		DEF_CODE(INTERNAL_SERVER_ERROR, "Internal Server Error", 500, "Unexpected server error");
		DEF_CODE(NOT_IMPLEMENTED, "Not Implemented", 501, "The server does not support the functionality required to fulfill the request");
		DEF_CODE(BAD_GATEWAY, "Bad Gateway", 502, "The server, acting as a gateway/proxy, received an invalid response from the upstream server it accessed in attempting to fulfill the request");
		DEF_CODE(SERVICE_UNAVAILABLE, "Service Unavailable", 503, "The server is currently unable to handle the request due to a temporary overloading or maintenance");
		DEF_CODE(GATEWAY_TIMEOUT, "Gateway Timeout", 504, "The server, acting as a gateway/proxy, did not receive a timely response from the upstream server it accessed in attempting to fulfill the request");
		DEF_CODE(HTTP_VERSION_NOT_SUPPORTED, "HTTP Version Not Supported", 505, "The server does not support, or refuses to support, the HTTP protocol version that was used in the request");
		DEF_CODE(VARIANT_ALSO_NEGOTIATES, "Variant Also Negotiates", 506, "The chosen variant resource is configured to engage in transparent content negotiation and is therefore not a proper end point in the negotiation process");
		DEF_CODE(INSUFFICIENT_STORAGE, "Insufficient Storage", 507, "The server was unable to process the request because it could not store the representation needed to successfully complete the request");
		DEF_CODE(LOOP_DETECTED, "Loop Detected", 508, "The server terminated an operation because it encountered an infinite loop while processing a request");
		DEF_CODE(BANDWIDTH_LIMIT_EXCEEDED, "Loop Detected", 509, "The amount of bandwidth allocated has been reached");
		DEF_CODE(NOT_EXTENDED, "Not Extended", 510, "The policy for accessing the resource has not been met in the request");
		DEF_CODE(NETWORK_AUTHENTICATION_REQUIRED, "Network Authentication Required", 511, "The client needs to authenticate to gain network access");
#undef DEF_CODE
	}

	namespace utils{
		template <>
		struct ToString<scl::http::StatusCode>{
			static const std::string STATUS, SEP, CF, SPECS;
			std::string operator()(const scl::http::StatusCode& status){
				return STATUS + std::to_string(status.status())
				+ SEP + status.name()
				+ CF + std::to_string(status.version()) + SPECS;
			}
		};

		const std::string ToString<scl::http::StatusCode>::STATUS = "Status ";
		const std::string ToString<scl::http::StatusCode>::SEP = ": ";
		const std::string ToString<scl::http::StatusCode>::CF = " (cf. ";
		const std::string ToString<scl::http::StatusCode>::SPECS = " specifications)";
	}
}