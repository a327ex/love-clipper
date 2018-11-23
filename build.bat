@echo off
cd C:\Users\gen\Desktop\dev\love-clipper\

call cl /EHsc /LD clipper\clipper.cpp love_clipper.cpp /I lua /I clipper /link /LIBPATH:lua lua51.lib /OUT:love_clipper.dll
copy love_clipper.dll C:\Users\aeon\Desktop\LOVE11\love_clipper.dll
del *.obj
del *.exp
del *.lib
