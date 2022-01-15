#!/bin/bash
clang-format -i src/*
clang-format -i src/include/*
clang-format -i src/linux/*
clang-format -i src/linux/game_server/*
clang-format -i src/linux/login_server/*
clang-format -i src/win32/*
clang-format -i src/win32/game_server/*
clang-format -i src/win32/login_server/*
clang-format -i src/storage/*