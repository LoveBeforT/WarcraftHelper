#pragma once

namespace base {
	template <typename object_type>
	struct singleton {
		singleton() = delete;
		static object_type& instance() {
			static object_type obj;
			return obj;
		}
	};
}
