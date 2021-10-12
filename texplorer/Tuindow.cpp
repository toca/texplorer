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
		//this->loopThread = std::thread([this]() {
			while (!this->stop)
			{
				try
				{
					this->Read();
					this->Render();
					Sleep(10);
				}
				catch (std::exception& ex)
				{
					fprintf(stderr, "Error!! %s\n", ex.what());
				}
			}
			this->screen->Close();
		//});
	}


	void Tuindow::Close()
	{
		this->stop = true;
		//if (this->loopThread.joinable())
		//{
		//	this->loopThread.join();
		//}
	}

	void Tuindow::SetCursor(short x, short y)
	{
		this->screen->SetCursor(x, y);
	}


	void Tuindow::Put(std::shared_ptr<Widget> widget)
	{
		widget->SetRect({
			0,
			0,
			long(this->screen->Col()),
			long(this->screen->Row())
		});
		widget->SetScreen(this->screen.get());
		this->widget = widget;
	}


	void Tuindow::AddKeyEventListener(KeyEventListener listener)
	{
		this->keyEventListeners.push_back(listener);
	}


	void Tuindow::Read()
	{
		auto screenEvent = this->screen->ReadInput();
		if (screenEvent.HasSizeEvent)
		{
			widget->SetRect({
				0,
				0,
				long(this->screen->Col()),
				long(this->screen->Row())
			});
		}
		if (screenEvent.HasKeyEvent)
		{
			for (int i = 0; i < screenEvent.KeyEvents.size(); i++)
			{
				this->OnKeyEvent(screenEvent.KeyEvents[i]);
			}
		}
	}

	void Tuindow::Render()
	{
		this->widget->Render();
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
