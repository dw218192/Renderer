#pragma once
#include <filesystem>

namespace ImGui {
	class FileDialogue {
		std::filesystem::path _cur_path;
		std::filesystem::path _result; // result path
		std::string _ext;
		bool _dirmode;
		char const* _name;
		char _buf[256];
		bool _is_open, _finished;

		void SetPath(std::filesystem::path const& newPath);
		void Reset();
		void DoWindow();
		bool IsDirectoryMode() const;
	public:
		FileDialogue(char const* name, bool dirmode, char const* ext = nullptr);
		bool IsOpen() const;
		bool Finished() const;
		bool HasResult() const;
		std::string GetPath() const;
		void FileDialogButton(char const* label);

		friend std::string OpenFileDialogue(FileDialogue& dialogue, char const* label);
	};
	std::string OpenFileDialogue(FileDialogue& dialogue, char const* label);
};