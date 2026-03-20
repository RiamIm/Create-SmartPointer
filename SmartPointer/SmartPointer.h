#pragma once
#include <utility>
#include <memory>

namespace hwang {
	template <typename T>
	class unique_ptr {
	public:
		unique_ptr(T* raw)
			: raw_(raw) {
		}

		~unique_ptr() {
			if (raw_)
				delete raw_;
		}

		// ===== 복사 생성 방지 =====
		unique_ptr(const unique_ptr&) = delete;
		unique_ptr& operator=(const unique_ptr&) = delete;

		// 소유권 이동 생성
		unique_ptr(unique_ptr&& other) noexcept {
			raw_ = other.raw_;
			other.raw_ = nullptr;
		}

		// 소유권 이동 대입
		unique_ptr& operator=(unique_ptr&& other) noexcept {
			// 자기 자신 대입 방지
			if (this != &other) {
				delete raw_;
				raw_ = other.raw_;
				other.raw_ = nullptr;
			}

			return*this;
		}

		// 주소 반환
		T* get() const { return raw_; }

		// 소유권 포기
		T* release() {
			auto* temp = raw_;
			raw_ = nullptr;
			return temp;
		}

		// 소유권 해제
		void reset(T* p = nullptr) {
			delete raw_;
			raw_ = p;
		}

		// ===== 포인터 기본 연산자 =====
		T& operator* () const { return *raw_; }
		T* operator-> () const { return raw_; }

	private:
		T* raw_;
	};

	// make_unique 헬퍼 함수
	template<typename T, typename... Args>
	unique_ptr<T> make_unique(Args&&... args) {
		return unique_ptr<T>(new T(std::forward<Args>(args)...));
	}
}