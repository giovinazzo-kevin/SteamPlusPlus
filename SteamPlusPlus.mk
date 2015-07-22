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
Date                   :=22/07/2015
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
IncludePath            :=  $(IncludeSwitch). $(IncludeSwitch)include 
IncludePCH             := 
RcIncludePath          := 
Libs                   := $(LibrarySwitch)lua 
ArLibs                 :=  "lua" 
LibPath                := $(LibraryPathSwitch). $(LibraryPathSwitch)lua-5.3.1/lib/ 

##
## Common variables
## AR, CXX, CC, AS, CXXFLAGS and CFLAGS can be overriden using an environment variables
##
AR       := K:/MinGW/bin/ar.exe rcu
CXX      := K:/MinGW/bin/g++.exe
CC       := K:/MinGW/bin/gcc.exe
CXXFLAGS :=  -g -O2 -Wall -std=c++11  $(Preprocessors)
CFLAGS   :=  -g -O2 -Wall $(Preprocessors)
ASFLAGS  := 
AS       := K:/MinGW/bin/as.exe


##
## User defined environment variables
##
CodeLiteDir:=D:\Program Files\CodeLite
Objects0=$(IntermediateDirectory)/src_SPP_Terminal.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_MainLoop.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_SPP_Script.cpp$(ObjectSuffix) 



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

$(IntermediateDirectory)/src_MainLoop.cpp$(ObjectSuffix): src/MainLoop.cpp $(IntermediateDirectory)/src_MainLoop.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "D:/Workspaces/CodeLite/C++/SteamPlusPlus/src/MainLoop.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_MainLoop.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_MainLoop.cpp$(DependSuffix): src/MainLoop.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_MainLoop.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_MainLoop.cpp$(DependSuffix) -MM "src/MainLoop.cpp"

$(IntermediateDirectory)/src_MainLoop.cpp$(PreprocessSuffix): src/MainLoop.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_MainLoop.cpp$(PreprocessSuffix) "src/MainLoop.cpp"

$(IntermediateDirectory)/src_SPP_Script.cpp$(ObjectSuffix): src/SPP_Script.cpp $(IntermediateDirectory)/src_SPP_Script.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "D:/Workspaces/CodeLite/C++/SteamPlusPlus/src/SPP_Script.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_SPP_Script.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_SPP_Script.cpp$(DependSuffix): src/SPP_Script.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_SPP_Script.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_SPP_Script.cpp$(DependSuffix) -MM "src/SPP_Script.cpp"

$(IntermediateDirectory)/src_SPP_Script.cpp$(PreprocessSuffix): src/SPP_Script.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_SPP_Script.cpp$(PreprocessSuffix) "src/SPP_Script.cpp"


-include $(IntermediateDirectory)/*$(DependSuffix)
##
## Clean
##
clean:
	$(RM) -r ./Debug/


