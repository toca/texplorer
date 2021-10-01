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

		void Push(std::shared_ptr<Widget> widget);

		void AddKeyEventListener(KeyEventListener listener);
	private:

		std::thread loopThread;
		std::unique_ptr<Screen> screen;
		std::vector < std::shared_ptr<Widget> >widgets;
		std::vector <KeyEventListener> keyEventListeners;
		bool stop;
		void Read();
		void Render();
		void OnKeyEvent(KEY_EVENT_RECORD keyEvent);
	};
}