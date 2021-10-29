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

	void Tuindow::Popup(std::shared_ptr<Widget> widget)
	{
		// TODO check fixed size
		this->popup = widget;
		this->popup->SetScreen(this->screen.get());
		auto hspace = this->screen->Col() - widget->FixedWidth();
		auto vspace = this->screen->Row() - widget->FixedHeight();
		this->popup->SetRect({
			long(hspace / 2),
			long(vspace / 2),
			long(hspace / 2) + widget->FixedWidth(),
			long(vspace / 2) + widget->FixedHeight()
		});
	}

	void Tuindow::Popdown()
	{
		if (this->popup)
		{
			this->popup.reset();
			this->popup = nullptr;
			this->widget->Refresh();
		}
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
			if (this->popup)
			{
				auto popW = popup->FixedWidth();
				auto popH = popup->FixedHeight();
				auto hspace = this->screen->Col() - popW;
				auto vspace = this->screen->Row() - popH;
				this->popup->SetRect({
					long(hspace / 2),
					long(vspace / 2),
					long(hspace / 2) + popW,
					long(vspace / 2) + popH
					});
			}
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
		if (this->popup)
		{
			this->popup->Render();
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
