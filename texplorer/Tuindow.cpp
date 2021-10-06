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
		auto inputs = this->screen->ReadInput();
		for (int i = 0; i < inputs.size(); i++)
		{
			this->OnKeyEvent(inputs[i]);
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
