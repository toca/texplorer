#include "View.h"
#include "Controller.h"
#include "Address.h"
#include "CurrentDir.h"
#include "Notification.h"
#include "Jumper.h"

#include "VerticalBox.h"
#include "HorizontalBox.h"
#include "SelectableList.h"
#include "DirectoryItem.h"
#include "DirectoryItemSize.h"
#include "DirectoryItemDate.h"
#include "Border.h"
#include "CharacterWidth.h"
#include "Spacer.h"


View::View(Controller* controller, std::shared_ptr<Address> address, std::shared_ptr<CurrentDir> dir, std::shared_ptr<Notification> notification, std::shared_ptr<Jumper> jumper)
	:controller(controller)
	,tuindow(std::make_unique<tuindow::Tuindow>())
	,addressLabel(std::make_shared<tuindow::Label>(L"", tuindow::Placement{ 1, 1, false, true }, std::make_shared<tuindow::Style>(tuindow::Color::WHITE, tuindow::Color::BLUE)))
	,address(address)
	,curDir(dir)
	,notification(notification)
	,jumper(jumper)
{
	// container
	auto vbox = std::make_shared<tuindow::VerticalBox>();

	// address bar
	auto hbox = std::make_shared<tuindow::HorizontalBox>();
	auto topLeft = std::make_shared<tuindow::Label>(L"+ ", tuindow::Placement{ 2, 1, true, true });
	auto topRight = std::make_shared<tuindow::Label>(L" +", tuindow::Placement{ 2, 1, true, true });
	auto pathLabel = std::make_shared<tuindow::Label>(L"Path: ", tuindow::Placement{ 6, 1, true, true }, tuindow::Style::Default()->Foreground(tuindow::Color::GREEN));
	hbox->Push(topLeft);
	hbox->Push(pathLabel);
	hbox->Push(this->addressLabel);
	hbox->Push(topRight);

	vbox->Push(hbox);

	// item list
	auto border  = std::make_shared<tuindow::Border>(L"|", L"", L"|", L"-", L"", L"", L"+", L"+");
	auto itemBox = std::make_shared<tuindow::HorizontalBox>();
	border->Put(itemBox);
	this->items = std::make_shared<tuindow::SelectableList>();
	itemBox->Push(this->items);
	this->sizes = std::make_shared<tuindow::SelectableList>(tuindow::Placement{ 8, 1, true, true });
	itemBox->Push(this->sizes);
	this->dates = std::make_shared<tuindow::SelectableList>(tuindow::Placement{ 20, 1, true, true });
	itemBox->Push(this->dates);
	vbox->Push(border);

	// operation
	auto man = std::make_shared<tuindow::Label>(
		L"Select[⇅ ] UpDir[←] IntoDir[→] RUN CMD[Esc] Open[^O] SortName[^N] SortSize[^S] SortDate:[^D] End:[^C]", 
		tuindow::Placement{ 1, 1, false, true }, 
		tuindow::Style::Default()->Foreground(tuindow::Color::GREEN)
	);
	vbox->Push(man);

	// status
	auto statusBox = std::make_shared<tuindow::HorizontalBox>();

	this->jumpMessage = std::make_shared<tuindow::Label>(this->jumper->Message(),
		tuindow::Placement{ 1, 1, false, true },
		tuindow::Style::Default()
	);
	statusBox->Push(this->jumpMessage);

	auto statusSpace = std::make_shared<tuindow::Spacer>(tuindow::Placement{ 1, 1, false, true }, tuindow::Style::Default());
	statusBox->Push(statusSpace);

	this->number = std::make_shared<tuindow::Label>(L"", tuindow::Placement{ 12, 1, true, true }, tuindow::Style::Default());
	statusBox->Push(this->number);
	vbox->Push(statusBox);

	this->tuindow->Put(vbox);
	this->tuindow->AddKeyEventListener([this](auto keyEvent) {
		this->OnKeyEvent(keyEvent);
	});
}

void View::Show()
{
	this->tuindow->Open();
}

void View::Close()
{
	this->tuindow->Close();
}

void View::OnNotificationChanged()
{
	if (!this->notification->ErrorOccurred())
	{
		this->tuindow->Popdown();
		return;
	}
	auto message = this->notification->Message();
	auto width = uint32_t(message.length() + 4 - sizeof('\0'));
	auto vbox = std::make_shared<tuindow::VerticalBox>(tuindow::Placement{ width, 5, true, true });

	std::wstring labelText = L"+- Error " + std::wstring(width - 10, L'-') + L"+";
	auto label = std::make_shared<tuindow::Label>(labelText, tuindow::Placement{ width, 1, true, true }, tuindow::Style::Default()->Background(tuindow::Color::BLUE));
	vbox->Push(label);

	std::wstring marginText = L"|" + std::wstring(width - 2, L' ') + L"|";
	auto margin1 = std::make_shared<tuindow::Label>(marginText, tuindow::Placement{ width, 1, true, true }, tuindow::Style::Default()->Background(tuindow::Color::BLUE));
	vbox->Push(margin1);

	std::wstring messageLine(L"| " + message + L" |");

	auto content = std::make_shared<tuindow::Label>(
		messageLine,
		tuindow::Placement{ uint32_t(message.size()), 1, true, true },
		tuindow::Style::Default()->Background(tuindow::Color::BLUE)
	);
	vbox->Push(content);

	auto margin2 = std::make_shared<tuindow::Label>(marginText, tuindow::Placement{ width, 1, true, true }, tuindow::Style::Default()->Background(tuindow::Color::BLUE));
	vbox->Push(margin2);

	std::wstring bottomLine(L"+" + std::wstring(width - 2, L'-') + L"+");
	auto bottom = std::make_shared<tuindow::Label>(bottomLine, tuindow::Placement{ width, 1, true, true }, tuindow::Style::Default()->Background(tuindow::Color::BLUE));
	vbox->Push(bottom);

	this->tuindow->Popup(vbox);
}

void View::OnAddressChanged()
{
	auto addr = this->address->Get();
	auto len = tuindow::Util::TextWidth(addr);;
	auto rect = this->addressLabel->GetRect();
	this->tuindow->SetCursor(rect.left + len, rect.top);
	this->addressLabel->Set(addr);
}

void View::OnItemChanged()
{
	std::vector<std::shared_ptr<tuindow::SelectableListItem>> itemList;
	std::vector<std::shared_ptr<tuindow::SelectableListItem>> sizeList;
	std::vector<std::shared_ptr<tuindow::SelectableListItem>> dateList;
	auto entories = this->curDir->GetItems();
	for (auto item : entories)
	{
		itemList.push_back(std::make_shared<DirectoryItem>(item));
		sizeList.push_back(std::make_shared<DirectoryItemSize>(item));
		dateList.push_back(std::make_shared<DirectoryItemDate>(item));
	}
	this->items->Set(itemList);
	this->sizes->Set(sizeList);
	this->dates->Set(dateList);

	this->itemCount = entories.size();
	this->OnStateChanged();
}

void View::Up()
{
	this->items->Up();
	this->sizes->Up();
	this->dates->Up();
	this->OnStateChanged();
}

void View::Down()
{
	this->items->Down();
	this->sizes->Down();
	this->dates->Down();
	this->OnStateChanged();
}

int View::Selected()
{
	return this->items->Selected();
}

void View::Select(int index)
{
	this->items->Select(index);
	this->sizes->Select(index);
	this->dates->Select(index);
	this->OnStateChanged();
}

void View::OnKeyEvent(KEY_EVENT_RECORD keyEvent)
{
	this->controller->OnKeyEvent(keyEvent);
}

void View::OnStateChanged()
{
	std::wstring numText = tuindow::Util::format(L"%d/%d", this->items->Selected() + 1, this->itemCount);
	std::wstring space(11 - numText.length(), L' ');
	this->number->Set(space + numText);
	this->jumpMessage->Set(this->jumper->Message());
}