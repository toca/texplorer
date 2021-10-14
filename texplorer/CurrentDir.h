#pragma once
#include <filesystem>
#include <functional>

enum class OrderBy {
	NAME, DATE, SIZE
};

class CurrentDir
{
public:
	void Change(const std::filesystem::path cd);
	void Up();
	void Into(int index);
	std::filesystem::path Absolute();
	std::vector<std::filesystem::directory_entry> GetItems();
	std::filesystem::path Get(int index);
	void SetOnChanged(std::function<void()> callback);
	void SortName();
	void SortDate();
	void SortSize();

private:
	void Sort(OrderBy by);
	std::filesystem::path cd;
	std::function<void()> callback;
	OrderBy orderBy = OrderBy::NAME;
	bool asc = true;
};

