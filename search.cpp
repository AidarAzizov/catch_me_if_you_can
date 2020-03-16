#include <set>
#include <iostream>
#include <list>
#include <string>
#include "dirent.h"
#include <algorithm>
#include <sys/stat.h>

using namespace std;

int		print_help()
{
	cout << "Usage: ./finder\t'path_to_directory' 'finder_peace'" << endl;
	cout << "Example: ./finder   \\Users  .jpeg" << endl;
	return (0);
}

bool	init(int argc, char **argv)
{
	DIR *dir;

	dir = opendir(argv[1]);
	if (argc != 3 || dir == NULL)
		return false;
	closedir(dir);
	return true;
}

bool	is_finder(char *str, string peace)
{
	char *p;

	p = strrchr(str, '.');
	if (!p)
		return false;
	return(strcmp(p, peace.c_str()) == 0);
}

bool	is_directory(const dirent *entry_dir)
{
	if (entry_dir->d_type == 4 && entry_dir->d_name[0] != '.')
		return (true);
	return (false);
}

void	search_jpeg_files(set<string> &photo_jpeg, const string path, const string peace)
{
	struct dirent *entry_dir;
	list<string> task;
	DIR *dir;

	task.push_back(path);
	for (auto it = task.begin(); it != task.end(); it++)
	{
		dir = opendir((*it).c_str());
		if (dir)
		{
			*it += "/";
			entry_dir = readdir(dir);
			while (entry_dir)
			{
				if (is_directory(entry_dir))
					task.push_back(*it + entry_dir->d_name);
				else if (is_finder(entry_dir->d_name, peace))
					photo_jpeg.emplace(entry_dir->d_name);
				entry_dir = readdir(dir);
			}
			closedir(dir);
		}
	}
}

int main(int argc, char **argv)
{
	set<string> photo_jpeg;

	if (!init(argc, argv))
		return print_help();
	search_jpeg_files(photo_jpeg, argv[1], argv[2]);
	cout << photo_jpeg.size() << endl;
	return (0);
}