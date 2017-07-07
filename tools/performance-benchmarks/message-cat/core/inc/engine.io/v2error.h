#pragma once

#include <iostream>
#include <string>
#include <system_error>

static const std::string nomsg = "";

	enum class v2_error {
		continue_request = 100,
		switching_protocols = 101,
		ok = 200,
		gateway_timeout = 504,
		version_not_supported = 505
	};

	/*error_category: */
	class dsav2_category_impl
		: public std::error_category
	{
	public:
		virtual const char* name() const noexcept {
			return "dsav2";
		}

		virtual std::string message(int ev) const {
			switch (ev)
			{
				case (int)  v2_error::continue_request:
					return "Continue";
				case (int)  v2_error::switching_protocols:
					return "Switching protocols";
				case (int)  v2_error::ok:
					return "OK";

				case (int)  v2_error::gateway_timeout:
					return "Gateway time-out";
				case (int)  v2_error::version_not_supported:
					return "DSA version not supported";
				default:
					return "Unknown DSA error";
			}
		}

	};

	const std::error_category& dsav2_category();

	const std::error_category& dsav2_category()
	{
		static dsav2_category_impl instance;
		return instance;
	}

	std::error_code make_error_code(v2_error e)
	{
		return std::error_code(
			static_cast<int>(e),
			dsav2_category());
	}

	std::error_condition make_error_condition(v2_error e)
	{
		return std::error_condition(
			static_cast<int>(e),
			dsav2_category());
	}

	namespace std
	{
		template <>
		struct is_error_code_enum<v2_error>
		: public true_type {};
	}

	namespace dsav2 {

		struct error {
			void operator()(std::error_code& ec, const std::string& emsg = nomsg) {
			//void reason(std::error_code& ec, const std::string& emsg = nomsg) {
				if (ec) {
					return;
				}
				if (ec == std::errc::permission_denied) {
					std::error_condition econd = std::system_category().default_error_condition(EPERM);

					std::cerr << econd.message() << " : " << emsg << std::endl;

					// throws and aborts at system init
					throw std::system_error(EPERM, dsav2_category());
				}
			}
		};
	}
