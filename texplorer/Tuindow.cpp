#include "Tuindow.h"
#include "Screen.h"
#include "Widget.h"

namespace tuindow
{
	Tuindow::Tuindow()
		: screen(std::make_unique<Screen>())
		, stop(false)
	{}


	void Tuindow::Open()
	{
		this->stop = false;
		this->loopThread = std::thread([this]() {
			while (!this->stop)
			{
				this->Read();
				this->Render();
				Sleep(10);
			}
		});
	}


	void Tuindow::Close()
	{
		this->stop = true;
		if (this->loopThread.joinable())
		{
			this->loopThread.join();
		}
	}


	void Tuindow::Push(std::shared_ptr<Widget> widget)
	{
		widget->SetRect({
			0,
			0,
			long(this->screen->Col()),
			long(this->screen->Row())
		});
		widget->SetScreen(this->screen.get());
		this->widgets.push_back(widget);
	}


	void Tuindow::AddKeyEventListener(KeyEventListener listener)
	{
		this->keyEventListeners.push_back(listener);
	}


	void Tuindow::Read()
	{
		this->OnKeyEvent(this->screen->ReadInput());
	}

	void Tuindow::Render()
	{
		for (auto w : this->widgets)
		{
			w->Render();
		}
		this->screen->Show();
	}

	void Tuindow::OnKeyEvent(KEY_EVENT_RECORD keyEvent)
	{
		for (auto& listener : this->keyEventListeners)
		{
			listener(keyEvent);
		}
	}

}
