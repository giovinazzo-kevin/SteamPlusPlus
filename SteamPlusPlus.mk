##
## Auto Generated makefile by CodeLite IDE
## any manual changes will be erased      
##
## Debug
ProjectName            :=SteamPlusPlus
ConfigurationName      :=Debug
WorkspacePath          := "D:\Workspaces\CodeLite\C++"
ProjectPath            := "D:\Workspaces\CodeLite\C++\SteamPlusPlus"
IntermediateDirectory  :=./Debug
OutDir                 := $(IntermediateDirectory)
CurrentFileName        :=
CurrentFilePath        :=
CurrentFileFullPath    :=
User                   :=Kappa
Date                   :=27/07/2015
CodeLitePath           :="D:\Program Files\CodeLite"
LinkerName             :=K:/MinGW/bin/g++.exe
SharedObjectLinkerName :=K:/MinGW/bin/g++.exe -shared -fPIC
ObjectSuffix           :=.o
DependSuffix           :=.o.d
PreprocessSuffix       :=.i
DebugSwitch            :=-g 
IncludeSwitch          :=-I
LibrarySwitch          :=-l
OutputSwitch           :=-o 
LibraryPathSwitch      :=-L
PreprocessorSwitch     :=-D
SourceSwitch           :=-c 
OutputFile             :=$(IntermediateDirectory)/$(ProjectName)
Preprocessors          :=
ObjectSwitch           :=-o 
ArchiveOutputSwitch    := 
PreprocessOnlySwitch   :=-E
ObjectsFileList        :="SteamPlusPlus.txt"
PCHCompileFlags        :=
MakeDirCommand         :=makedir
RcCmpOptions           := 
RcCompilerName         :=K:/MinGW/bin/windres.exe
LinkOptions            :=  
IncludePath            :=  $(IncludeSwitch). $(IncludeSwitch)include $(IncludeSwitch)lua-5.3.1/include $(IncludeSwitch)mingw-threads/include 
IncludePCH             := 
RcIncludePath          := 
Libs                   := $(LibrarySwitch)lua $(LibrarySwitch)steamclient 
ArLibs                 :=  "lua" "steamclient" 
LibPath                := $(LibraryPathSwitch). $(LibraryPathSwitch)lua-5.3.1/lib/ $(LibraryPathSwitch)opensteamworks/lib/Win32/ 

##
## Common variables
## AR, CXX, CC, AS, CXXFLAGS and CFLAGS can be overriden using an environment variables
##
AR       := K:/MinGW/bin/ar.exe rcu
CXX      := K:/MinGW/bin/g++.exe
CC       := K:/MinGW/bin/gcc.exe
CXXFLAGS :=  -g -O2 -Wall -std=c++11 -isystem opensteamworks/include  $(Preprocessors)
CFLAGS   :=  -g -O2 -Wall $(Preprocessors)
ASFLAGS  := 
AS       := K:/MinGW/bin/as.exe


##
## User defined environment variables
##
CodeLiteDir:=D:\Program Files\CodeLite
Objects0=$(IntermediateDirectory)/src_SPP_Terminal.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_Main.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_SPP_SteamPlusPlus.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_SPP_Lua.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_SPP_OpenSteamworks.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_SPP_LuaSandbox.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_SPP_Callbacks.cpp$(ObjectSuffix) 



Objects=$(Objects0) 

##
## Main Build Targets 
##
.PHONY: all clean PreBuild PrePreBuild PostBuild
all: $(OutputFile)

$(OutputFile): $(IntermediateDirectory)/.d $(Objects) 
	@$(MakeDirCommand) $(@D)
	@echo "" > $(IntermediateDirectory)/.d
	@echo $(Objects0)  > $(ObjectsFileList)
	$(LinkerName) $(OutputSwitch)$(OutputFile) @$(ObjectsFileList) $(LibPath) $(Libs) $(LinkOptions)

$(IntermediateDirectory)/.d:
	@$(MakeDirCommand) "./Debug"

PreBuild:


##
## Objects
##
$(IntermediateDirectory)/src_SPP_Terminal.cpp$(ObjectSuffix): src/SPP_Terminal.cpp $(IntermediateDirectory)/src_SPP_Terminal.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "D:/Workspaces/CodeLite/C++/SteamPlusPlus/src/SPP_Terminal.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_SPP_Terminal.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_SPP_Terminal.cpp$(DependSuffix): src/SPP_Terminal.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_SPP_Terminal.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_SPP_Terminal.cpp$(DependSuffix) -MM "src/SPP_Terminal.cpp"

$(IntermediateDirectory)/src_SPP_Terminal.cpp$(PreprocessSuffix): src/SPP_Terminal.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_SPP_Terminal.cpp$(PreprocessSuffix) "src/SPP_Terminal.cpp"

$(IntermediateDirectory)/src_Main.cpp$(ObjectSuffix): src/Main.cpp $(IntermediateDirectory)/src_Main.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "D:/Workspaces/CodeLite/C++/SteamPlusPlus/src/Main.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_Main.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_Main.cpp$(DependSuffix): src/Main.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_Main.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_Main.cpp$(DependSuffix) -MM "src/Main.cpp"

$(IntermediateDirectory)/src_Main.cpp$(PreprocessSuffix): src/Main.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_Main.cpp$(PreprocessSuffix) "src/Main.cpp"

$(IntermediateDirectory)/src_SPP_SteamPlusPlus.cpp$(ObjectSuffix): src/SPP_SteamPlusPlus.cpp $(IntermediateDirectory)/src_SPP_SteamPlusPlus.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "D:/Workspaces/CodeLite/C++/SteamPlusPlus/src/SPP_SteamPlusPlus.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_SPP_SteamPlusPlus.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_SPP_SteamPlusPlus.cpp$(DependSuffix): src/SPP_SteamPlusPlus.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_SPP_SteamPlusPlus.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_SPP_SteamPlusPlus.cpp$(DependSuffix) -MM "src/SPP_SteamPlusPlus.cpp"

$(IntermediateDirectory)/src_SPP_SteamPlusPlus.cpp$(PreprocessSuffix): src/SPP_SteamPlusPlus.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_SPP_SteamPlusPlus.cpp$(PreprocessSuffix) "src/SPP_SteamPlusPlus.cpp"

$(IntermediateDirectory)/src_SPP_Lua.cpp$(ObjectSuffix): src/SPP_Lua.cpp $(IntermediateDirectory)/src_SPP_Lua.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "D:/Workspaces/CodeLite/C++/SteamPlusPlus/src/SPP_Lua.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_SPP_Lua.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_SPP_Lua.cpp$(DependSuffix): src/SPP_Lua.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_SPP_Lua.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_SPP_Lua.cpp$(DependSuffix) -MM "src/SPP_Lua.cpp"

$(IntermediateDirectory)/src_SPP_Lua.cpp$(PreprocessSuffix): src/SPP_Lua.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_SPP_Lua.cpp$(PreprocessSuffix) "src/SPP_Lua.cpp"

$(IntermediateDirectory)/src_SPP_OpenSteamworks.cpp$(ObjectSuffix): src/SPP_OpenSteamworks.cpp $(IntermediateDirectory)/src_SPP_OpenSteamworks.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "D:/Workspaces/CodeLite/C++/SteamPlusPlus/src/SPP_OpenSteamworks.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_SPP_OpenSteamworks.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_SPP_OpenSteamworks.cpp$(DependSuffix): src/SPP_OpenSteamworks.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_SPP_OpenSteamworks.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_SPP_OpenSteamworks.cpp$(DependSuffix) -MM "src/SPP_OpenSteamworks.cpp"

$(IntermediateDirectory)/src_SPP_OpenSteamworks.cpp$(PreprocessSuffix): src/SPP_OpenSteamworks.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_SPP_OpenSteamworks.cpp$(PreprocessSuffix) "src/SPP_OpenSteamworks.cpp"

$(IntermediateDirectory)/src_SPP_LuaSandbox.cpp$(ObjectSuffix): src/SPP_LuaSandbox.cpp $(IntermediateDirectory)/src_SPP_LuaSandbox.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "D:/Workspaces/CodeLite/C++/SteamPlusPlus/src/SPP_LuaSandbox.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_SPP_LuaSandbox.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_SPP_LuaSandbox.cpp$(DependSuffix): src/SPP_LuaSandbox.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_SPP_LuaSandbox.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_SPP_LuaSandbox.cpp$(DependSuffix) -MM "src/SPP_LuaSandbox.cpp"

$(IntermediateDirectory)/src_SPP_LuaSandbox.cpp$(PreprocessSuffix): src/SPP_LuaSandbox.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_SPP_LuaSandbox.cpp$(PreprocessSuffix) "src/SPP_LuaSandbox.cpp"

$(IntermediateDirectory)/src_SPP_Callbacks.cpp$(ObjectSuffix): src/SPP_Callbacks.cpp $(IntermediateDirectory)/src_SPP_Callbacks.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "D:/Workspaces/CodeLite/C++/SteamPlusPlus/src/SPP_Callbacks.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_SPP_Callbacks.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_SPP_Callbacks.cpp$(DependSuffix): src/SPP_Callbacks.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_SPP_Callbacks.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_SPP_Callbacks.cpp$(DependSuffix) -MM "src/SPP_Callbacks.cpp"

$(IntermediateDirectory)/src_SPP_Callbacks.cpp$(PreprocessSuffix): src/SPP_Callbacks.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_SPP_Callbacks.cpp$(PreprocessSuffix) "src/SPP_Callbacks.cpp"


-include $(IntermediateDirectory)/*$(DependSuffix)
##
## Clean
##
clean:
	$(RM) -r ./Debug/


