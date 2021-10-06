#include "View.h"
#include "Controller.h"
#include "Address.h"
#include "CurrentDir.h"

#include "VerticalBox.h"
#include "HorizontalBox.h"
#include "SelectableList.h"
#include "DirectoryItem.h"
#include "DirectoryItemSize.h"
#include "DirectoryItemDate.h"

View::View(Controller* controller, std::shared_ptr<Address> address, std::shared_ptr<CurrentDir> dir)
	:controller(controller)
	,tuindow(std::make_unique<tuindow::Tuindow>())
	,addressLabel(std::make_shared<tuindow::Label>(L"", tuindow::Placement{ 1, 1, false, true }, std::make_shared<tuindow::Style>(tuindow::Color::WHITE, tuindow::Color::BLUE)))
	,address(address)
	,curDir(dir)
{
	// container
	auto vbox = std::make_shared<tuindow::VerticalBox>();

	// address bar
	auto hbox = std::make_shared<tuindow::HorizontalBox>();
	auto pathLabel = std::make_shared<tuindow::Label>(L"Path: ", tuindow::Placement{ 6, 1, true, true }, tuindow::Style::Default()->Foreground(tuindow::Color::GREEN));
	hbox->Push(pathLabel);
	hbox->Push(this->addressLabel);

	vbox->Push(hbox);

	// item list
	auto itemBox = std::make_shared<tuindow::HorizontalBox>();
	this->items = std::make_shared<tuindow::SelectableList>();
	itemBox->Push(this->items);
	this->sizes = std::make_shared<tuindow::SelectableList>(tuindow::Placement{ 8, 1, true, true });
	itemBox->Push(this->sizes);
	this->dates = std::make_shared<tuindow::SelectableList>(tuindow::Placement{ 20, 1, true, true });
	itemBox->Push(this->dates);
	vbox->Push(itemBox);



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

void View::OnAddressChanged()
{
	this->addressLabel->Set(this->address->Get());
}

void View::OnItemChanged()
{
	std::vector<std::shared_ptr<tuindow::SelectableListItem>> itemList;
	std::vector<std::shared_ptr<tuindow::SelectableListItem>> sizeList;
	std::vector<std::shared_ptr<tuindow::SelectableListItem>> dateList;
	for (auto item : this->curDir->GetItems())
	{
		itemList.push_back(std::make_shared<DirectoryItem>(item));
		sizeList.push_back(std::make_shared<DirectoryItemSize>(item));
		dateList.push_back(std::make_shared<DirectoryItemDate>(item));
	}
	this->items->Set(itemList);
	this->sizes->Set(sizeList);
	this->dates->Set(dateList);
}

void View::Up()
{
	this->items->Up();
	this->sizes->Up();
	this->dates->Up();
}

void View::Down()
{
	this->items->Down();
	this->sizes->Down();
	this->dates->Down();
}

void View::OnKeyEvent(KEY_EVENT_RECORD keyEvent)
{
	this->controller->OnKeyEvent(keyEvent);
}
