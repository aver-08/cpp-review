#include <iostream>
#include <sstream>
#include <array>
#include <vector>

enum class Command {
	READ, CHEER
};

using namespace std::literals;

struct CommandDescription {
	CommandDescription() = default;
	std::string command;
	int person_id = 0,
		pages_count = 0;

	bool operator==(const Command& rhs) {
		if (this->command == "READ"s) {
			return rhs == Command::READ;
		}
		else if (this->command == "CHEER"s) {
			return rhs == Command::CHEER;
		}
		return false;
	}
	bool operator!=(const Command& rhs) {
		return !(*this == rhs);
	}
};

class Ebook {
public:
	Ebook() : readers_(100000, 0), pages_read_(1000, 0) {}

	void CommandRead(const CommandDescription& cmd) {
		auto reader = cmd.person_id - 1;
		auto& pages = readers_[reader];
		if (pages == 0)
		{
			++readers_count_;
		}
		int old_count_pages = pages;
		pages = cmd.pages_count;

		for (int i = old_count_pages; i < cmd.pages_count; ++i) {
			++pages_read_[i];
		}
	}
	double CommandCheer(const CommandDescription& cmd) {
		const auto& pages = readers_[cmd.person_id - 1];
		if (pages == 0) {
			return 0.;
		}
		if (pages > 0 && readers_count_ == 1) {
			return 1.;
		}
		return static_cast<double>(readers_count_ - pages_read_[pages - 1])
			/ static_cast<double>(readers_count_ - 1);
	}

private:
	int readers_count_ = 0;
	std::vector<int> readers_; // { 100000, 0 };
	std::vector<int> pages_read_; // { 1000, 0 };
};

CommandDescription ReadCommand(std::istream& in) {
	std::string command_line;
	std::getline(in, command_line);
	std::istringstream in_str(std::move(command_line));

	CommandDescription cmd;
	in_str >> cmd.command >> cmd.person_id >> cmd.pages_count;
	return cmd;
}

int main() {
	int queries_count = 0;
	std::cin >> queries_count;

	Ebook ebook;

	for (int i = 0; i <= queries_count; ++i) {
		auto cmd = ReadCommand(std::cin);

		if (cmd == Command::READ) {
			ebook.CommandRead(cmd);
			continue;
		}

		if (cmd == Command::CHEER) {
			std::cout << ebook.CommandCheer(cmd) << std::endl;
		}
	}
}