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

		// ===== 유틸리티 =====
		T& operator* () const { return *raw_; }
		T* operator-> () const { return raw_; }
		T* get() const { return raw_; }

	private:
		T* raw_;
	};

	// make_unique 헬퍼 함수
	template<typename T, typename... Args>
	unique_ptr<T> make_unique(Args&&... args) {
		return unique_ptr<T>(new T(std::forward<Args>(args)...));
	}

	template<typename T>
	class shared_ptr {
	public:
		shared_ptr(T* p_raw)
			: raw_(p_raw), refCount_(new int(1)) {
		}

		~shared_ptr() {
			--*refCount_;
			if (*refCount_ == 0) {
				clear(*this);
			}
		}

		// 복사 생성자
		shared_ptr(const shared_ptr& other) {
			this->raw_ = other.raw_;
			this->refCount_ = other.refCount_;
			++*this->refCount_;
		}

		// 복사 대입
		shared_ptr& operator=(const shared_ptr& other) {
			--*this->refCount_;
			if (*this->refCount_ == 0) {
				clear(*this);
			}
			
			this->raw_ = other.raw_;
			this->refCount_ = other.refCount_;
			++*this->refCount_;

			return *this;
		}

		// 이동 생성
		shared_ptr(shared_ptr&& other) {
			this->raw_ = other.raw_;
			this->refCount_ = other.refCount_;
			release(other);
		}

		// 이동 대입
		shared_ptr& operator=(shared_ptr&& other) {
			if (this != &other) {
				--*this->refCount_;
				if (*this->refCount_ == 0) {
					clear(*this);
				}

				this->raw_ = other.raw_;
				this->refCount_ = other.refCount_;
				release(other);
			}
			
			return *this;
		}

		// ===== 유틸리티 =====
		T* get() const { return raw_; }
		T& operator* () const { return *raw_; }
		T* operator-> () const { return raw_; }
		int use_count() const { return *refCount_; }

	private:
		T* raw_;
		int* refCount_;

		void clear(shared_ptr& ptr) {
			delete ptr.raw_;
			delete ptr.refCount_;
		}

		void release(shared_ptr& ptr) {
			ptr.raw_ = nullptr;
			ptr.refCount_ = nullptr;
		}
	};

	// make_shared 핼퍼 함수
	template<typename T, typename... Args>
	shared_ptr<T> make_shared(Args&&... args) {
		return shared_ptr<T>(new T(std::forward<Args>(args)...));
	}
}