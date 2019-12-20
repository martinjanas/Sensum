# Translations

### Creating new translation

1. Open project.
2. Right click on `render` -> `Add` -> `New Item...`
3. Call it `strings_*lang code*.cpp` and put it in `render` folder.
4. Copy contents of `strings_template.cpp` to this file.
5. Do 2-3 for `strings_*lang code*.h`
6. Copy contents of `strings_template.h` to this file.
7. Enter translated strings into corresponding lines.
8. Rename function `settolang()` in `strings_template.cpp` and `strings_template.h` to `setto`\*lang code\*`()`

`Example:` for French you would rename it to `settofr()`

9. Open `render\tabs\misc_tab.cpp:289` and add your language to this list.
10. Open `render\string.cpp` and go to the end.
11. Edit `updatelang()` by adding new if statement with `list index - 1`. `Example:` if you set list to
```c++
static const char* languages[] = {
  "English",
  "Russian",
  "French"
};
```
you would add this if statement:
```c++
if (globals::language == 2)
  strings::settofr();
```
12. Go to the top and include the file created in step 6.
