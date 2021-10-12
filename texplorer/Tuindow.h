#pragma once
#include <Windows.h>
#include <thread>
#include <memory>
#include <functional>
#include "Screen.h"

namespace tuindow
{
	// proto
	class Widget;

	using KeyEventListener = std::function<void(KEY_EVENT_RECORD)>;
	/** TUI Window class */
	class Tuindow
	{
	public:
		Tuindow();
		void Open();
		void Close();
		void SetCursor(short x, short y);

		void Put(std::shared_ptr<Widget> widget);

		void AddKeyEventListener(KeyEventListener listener);
	private:

		std::thread loopThread;
		std::unique_ptr<Screen> screen;
		std::shared_ptr<Widget> widget;
		std::vector <KeyEventListener> keyEventListeners;
		bool stop;
		void Read();
		void Render();
		void OnKeyEvent(KEY_EVENT_RECORD keyEvent);
	};
}