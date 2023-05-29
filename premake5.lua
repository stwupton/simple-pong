workspace 'Simple Pong'
  configurations { 'Debug', 'Release' }
  platforms { 'Win64' }
  location 'build'
  
project 'simple-pong'
  kind 'WindowedApp'
  language 'C++'
  cppdialect 'C++17'
  files { 'src/sdl_entry.cpp' }
  includedirs { 'src', 'include' }
  libdirs { 'lib' }
  
  filter 'configurations:Release'
    defines { 'NDEBUG' }
    optimize 'On'
  
  filter 'configurations:Debug'
    symbols 'On'

  filter 'platforms:Win64'
    system 'Windows'
    architecture 'x86_64'
  
  filter { 'system:Windows', 'toolset:clang' }
    linkoptions '-Xlinker /subsystem:windows'
  
  filter { 'configurations:Debug', 'system:Windows', 'toolset:clang' }
    linkoptions '-g'
  
  filter 'system:Windows'
    links { 'shell32', 'SDL2main', 'SDL2', 'opengl32' }
    defines { 'WIN32', 'UNICODE' }