#pragma once

#include <queue>
#include <Windows.h>

class Mouse
{
	friend class Window;

public:
	class Event
	{
	public:
		enum class Type
		{
			LPress,
			LRelease,
			RPress,
			RRelease,
			WheelUp,
			WheelDown,
			Move,
			Enter,
			Leave,
			Invalid
		};

	private:
		Type type;
		int x;
		int y;
		bool leftIsPressed;
		bool rightIsPressed;

	public:
		Event() noexcept
			:
			type(Type::Invalid),
			x(0), y(0),
			leftIsPressed(false),
			rightIsPressed(false)
		{
		}

		Event(Type type, const Mouse& parent) noexcept
			:
			type(type),
			x(parent.x),
			y(parent.y),
			leftIsPressed(parent.leftIsPressed),
			rightIsPressed(parent.rightIsPressed)
		{
		}

		bool IsValid() const noexcept
		{
			return type != Type::Invalid;
		}

		Type GetType() const noexcept
		{
			return type;
		}

		std::pair<int, int> GetPos() const noexcept
		{
			return { x, y };
		}

		int GetPosX() const noexcept
		{
			return x;
		}

		int GetPosY() const noexcept
		{
			return y;
		}

		bool LeftIsPressed() const noexcept
		{
			return leftIsPressed;
		}
		bool RightIsPressed() const noexcept
		{
			return rightIsPressed;
		}
	};

public:
	Mouse() = default;
	Mouse(const Mouse&) = delete;
	Mouse& operator=(const Mouse&) = delete;

	std::pair<int, int> GetPos() const noexcept;
	int GetPosX() const noexcept;
	int GetPosY() const noexcept;
	bool LeftIsPressed() const noexcept;
	bool RightIsPressed() const noexcept;
	bool IsInWindow() const noexcept;
	Mouse::Event Read() noexcept;
	bool IsEmpty() const noexcept;
	void Flush() noexcept;

private:
	void OnMouseMove(int x, int y) noexcept;
	void OnMouseEnter() noexcept;
	void OnMouseLeave() noexcept;
	void OnLeftPressed(int x, int y) noexcept;
	void OnLeftReleased(int x, int y) noexcept;
	void OnRightPressed(int x, int y) noexcept;
	void OnRightReleased(int x, int y) noexcept;
	void OnWheelUp(int x, int y) noexcept;
	void OnWheelDown(int x, int y) noexcept;
	void OnWheelDelta(int x, int y, int delta) noexcept;

	void TrimBuffer() noexcept;

private:
	static constexpr unsigned int bufferSize = 16u;
	int x;
	int y;
	bool leftIsPressed = false;
	bool rightIsPressed = false;
	bool isInWindow = false;
	int wheelDeltaCarry = 0;
	std::queue<Event> buffer;
};