from argparse import ArgumentParser
from io import TextIOWrapper
import os
from typing import Iterable, Optional, Tuple

REWARD_NS_NAME = "Rewards"
STATES_NS_NAME = "States"
AP_NS_NAME = "AP"
OB_NS_NAME = "OB"

OBS_BUILDER_FOLDER_NAME = "ObsBuilders"
ACT_PARSER_FOLDER_NAME = "ActionParsers"
REWARDS_FOLDER_NAME = "Rewards"
STATES_FOLDER_NAME = "StateSetters"

def start_macro(name: str, sub: str = "") -> str:
    return "START_" + name.upper() + f"{'_' + sub.upper() if sub.strip() != '' else ''}_NS"

def use_macro(name: str, sub: str = "") -> str:
    return "USE_" + name.upper() + f"{'_' + sub.upper() if sub.strip() != '' else ''}_NS"

def _add_include(include_path: str, file: TextIOWrapper):
    file.write(f"#include <{include_path}>\n")
    
def _add_define(var_name: str, var_value: str, file: TextIOWrapper):
    file.write(f"#define {var_name} {var_value}\n")
    
def _add_comment(comment: str, file: TextIOWrapper):
    file.write("//" + comment + " (Auto-generated comment)\n")   
    
def create_method_declaration(name: str, is_virtual: bool, return_type: str = "", args: Iterable[Tuple[bool, str, str, Optional[str]]] = [], is_override: bool = False, is_const = False) -> str:
    content = ("const " if is_const else "") + "virtual" if is_virtual else ""
    content += " " + return_type if return_type.strip() != "" else "void"
    content += " " + name + "("
    for arg in args:
        c, t, n, v = arg
        content += ("const " if c else "") + t + " " + n + (" = " + v if v else "") + ", "
        
    content = content[:-2]
    content += ")"
    content += " override" if is_override else ""
    content += ";\n"
    
    return content

def create_ap_file(path: str, name: str, header: bool):
    capitalized_name = name[0].upper() + name[1:]
    if header:
        with open(os.path.join(path, name + "ActionParser.h"), "x") as f:
            _add_include(f"{name}/{name + 'Utils.h'}", f)
            _add_include("RLGymSim_CPP/Utils/ActionParsers/DiscreteAction.h", f)
            
            f.write("\n")
            f.write("using namespace RLGSC;\n\n")
            
            f.write(start_macro(name, AP_NS_NAME) + "\n\n")
            
            _add_comment("Autogenerated, inherits DiscreteAction", f)
            f.write(f"class {capitalized_name}ActionParser: public DiscreteAction" + "{\n")
            f.write("};\n\n")
            f.write(f"END_{name.upper()}_NS\n")
            
    else:
        folder_name = path.split("\\")[-1]
        
        with open(os.path.join(path, name + "ActionParser.cpp"), "x") as f:
            _add_include(f"{name}/{name + 'Utils.h'}", f)
            _add_include(f"{name}/{folder_name}/{name + 'ActionParser.h'}", f)
            
def create_ob_file(path: str, name: str, header: bool):
    capitalized_name = name[0].upper() + name[1:]
    if header:
        with open(os.path.join(path, name + "ObsBuilder.h"), "x") as f:
            _add_include(f"{name}/{name + 'Utils.h'}", f)
            _add_include("RLGymSim_CPP/Utils/OBSBuilders/DefaultOBSPadded.h", f)
            
            f.write("\n")
            f.write("using namespace RLGSC;\n\n")
            
            f.write(start_macro(name, OB_NS_NAME) + "\n\n")
            
            _add_comment("Autogenerated, inherits DefaultOBSPadded", f)
            f.write(f"class {capitalized_name}ObsBuilder: public DefaultOBSPadded" + "{\n")
            f.write("};\n\n")
            f.write(f"END_{name.upper()}_NS\n")
    else:
        folder_name = path.split("\\")[-1]
        
        with open(os.path.join(path, name + "ObsBuilder.cpp"), "x") as f:
            _add_include(f"{name}/{name + 'Utils.h'}", f)
            _add_include(f"{name}/{folder_name}/{name + 'ObsBuilder.h'}", f)
           

def create_state_file(path: str, name: str, header: bool):
    capitalized_name = name[0].upper() + name[1:]
    
    if header:
        with open(os.path.join(path, name + "StateSetter.h"), "x") as f:
            _add_include(f"{name}/{name + 'Utils.h'}", f)
            _add_include("RLGymSim_CPP/Utils/StateSetters/StateSetter.h", f)
            f.write("\n")
            
            f.write("using namespace RLGSC;\n\n")
            
            f.write(start_macro(name, STATES_NS_NAME) + "\n\n")
            f.write(f"class {capitalized_name}StateSetter: public StateSetter" + "{\n")
            f.write("public:\n")
            f.write(f"\t{capitalized_name}StateSetter()" + "{};\n")
            f.write("\t" + create_method_declaration(
                name="ResetState", 
                is_virtual=True, 
                return_type="GameState",
                args=[
                    [False, "Arena*", "arena", None]
                ],
                is_override=True,
                is_const=False
            )) 
            
            f.write("};\n\n")
            f.write(f"END_{name.upper()}_NS\n")
    else:
        
        folder_name = path.split("\\")[-1]
        
        with open(os.path.join(path, name + "StateSetter.cpp"), "x") as f:
            _add_include(f"{name}/{name + 'Utils.h'}", f)
            _add_include(f"{name}/{folder_name}/{name + 'StateSetter.h'}", f)
            f.write("\n")
            
            f.write(use_macro(name, STATES_NS_NAME) + ";\n\n")
            f.write(f"GameState {capitalized_name}StateSetter::ResetState(Arena* arena)" + "\n" +
                    "{" + "\n" + \
                    "\t//Insert your state setter logic here" + "\n\n" + \
                    "\treturn GameState(arena);" + "\n" + \
                    "}\n")

def create_reward_file(path: str, name: str, header: bool):
    capitalized_name = name[0].upper() + name[1:]
    
    if header:
        with open(os.path.join(path, name + "Reward.h"), "x") as f:
            _add_include(f"{name}/{name + 'Utils.h'}", f)
            _add_include("Logging/LoggableReward.h", f)
            f.write("\n")
            
            f.write("using namespace RLGSC;\n\n")
            f.write(use_macro("LOGGING") + ";\n")
            
            f.write(start_macro(name, REWARD_NS_NAME) + "\n\n")
            f.write(f"class {capitalized_name}Reward: public LoggableReward" + "{\n")
            f.write("public:\n")
            f.write(f"\t{capitalized_name}Reward(std::string name = \"{capitalized_name}\"): LoggableReward(name)" + "{};\n")
            f.write("\t" + create_method_declaration(
                name="GetReward", 
                is_virtual=True, 
                return_type="float",
                args=[
                    [True, "PlayerData&", "player", None],
                    [True, "GameState&", "state", None],
                    [True, "Action&", "prevAction", None] 
                ],
                is_override=True,
                is_const=False
            )) 
            
            f.write("};\n\n")
            f.write(f"END_{name.upper()}_NS\n")
    else:
        
        folder_name = path.split("\\")[-1]
        
        with open(os.path.join(path, name + "Reward.cpp"), "x") as f:
            _add_include(f"{name}/{name + 'Utils.h'}", f)
            _add_include(f"{name}/{folder_name}/{name + 'Reward.h'}", f)
            f.write("\n")
            
            f.write(use_macro(name, REWARD_NS_NAME) + ";\n\n")
            f.write(f"float {capitalized_name}Reward::GetReward(const PlayerData& player, const GameState& state, const Action& prevAction)" + "\n" +
                    "{" + "\n" + \
                    "\t//Insert your reward logic here" + "\n\n" + \
                    "\treturn this->ComputeReward();" + "\n" + \
                    "}\n")
    
def get_all_start_macros(name) -> Iterable[str]:   
    return \
        start_macro(name), \
        start_macro(name, REWARD_NS_NAME), \
        start_macro(name, STATES_NS_NAME), \
        start_macro(name, AP_NS_NAME), \
        start_macro(name, OB_NS_NAME)
 
def get_all_use_macros(name) -> Iterable[str]:   
    return \
        use_macro(name, REWARD_NS_NAME), \
        use_macro(name, STATES_NS_NAME), \
        use_macro(name, AP_NS_NAME), \
        use_macro(name, OB_NS_NAME)
               
    
def _add_namespaces_and_uses(name: str, file: TextIOWrapper):
    ns_var_name = name.upper() + "_NS"
    
    all_ns = [REWARD_NS_NAME, STATES_NS_NAME, AP_NS_NAME, OB_NS_NAME]
    
    file.write("\n")
    _add_comment("Base namespace utils", file)
    _add_define(ns_var_name, name, file)
    _add_define("START_" + name.upper() + "_NS", f"START_VOID_SUB_NS({ns_var_name})", file)
    _add_define("END_" + name.upper() + "_NS", "END_NS", file)
    
    file.write("\n")
    _add_comment("Sub namespaces start macros", file)
    for ns in all_ns:
        _add_define(start_macro(name, ns), f"START_VOID_SUB_NS({ns_var_name}::{ns})", file)
    
    file.write("\n")
    _add_comment("Sub namespaces use macros", file)
    for ns in all_ns:
        _add_define(use_macro(name, ns), f"USE_VOID_SUB_NS({ns_var_name}::{ns})", file)
    

def create_folder_structure(path: str, name: str, is_header: bool):
    
    path_to_name = os.path.join(path, name)
    utils_file_name = name + "Utils." + ("h" if is_header else "cpp")

    
    if os.path.exists(path_to_name):
        print(f"Directory \"{path_to_name}\" already exists, can't create")
        return
    
    os.mkdir(path_to_name)
    os.mkdir(os.path.join(path_to_name, OBS_BUILDER_FOLDER_NAME))
    os.mkdir(os.path.join(path_to_name, ACT_PARSER_FOLDER_NAME))
    os.mkdir(os.path.join(path_to_name, REWARDS_FOLDER_NAME))
    os.mkdir(os.path.join(path_to_name, STATES_FOLDER_NAME))
    
    create_reward_file(os.path.join(path_to_name, REWARDS_FOLDER_NAME), name, is_header)
    create_state_file(os.path.join(path_to_name, STATES_FOLDER_NAME), name, is_header)
    create_ap_file(os.path.join(path_to_name, ACT_PARSER_FOLDER_NAME), name, is_header)
    create_ob_file(os.path.join(path_to_name, OBS_BUILDER_FOLDER_NAME), name, is_header)
    
    with open(os.path.join(path_to_name, utils_file_name), "x") as f:
        if not is_header:
            # I assume the header is already created
            _add_include(f"{name}/{utils_file_name[:-4] + '.h'}", f)
        else:
            _add_include("Utils/VoidUtils.h", f)
            _add_namespaces_and_uses(name, f)
        
            
def create_main(path, name):
    path_to_name = os.path.join(path, name + "Main.cpp")
    utils_file_name = name + "Utils.h"
    
    if os.path.exists(path_to_name):
        print(f"\"{path_to_name}\" already exists, can't create")
        return
    
    with open(path_to_name, "x") as f:
        _add_include(f"{name}/{utils_file_name[:-2] + '.h'}", f)
        _add_include(f"{name}/{ACT_PARSER_FOLDER_NAME}/{name}ActionParser.h", f)
        _add_include(f"{name}/{STATES_FOLDER_NAME}/{name}StateSetter.h", f)
        _add_include(f"{name}/{REWARDS_FOLDER_NAME}/{name}Reward.h", f)
        _add_include(f"{name}/{OBS_BUILDER_FOLDER_NAME}/{name}ObsBuilder.h", f)
            
        f.write("\n")
        _add_comment("Uses", f)
        for use in get_all_use_macros(name):
            f.write(use + ";\n")
        f.write("\n")
        
        
        content = ""
        with open(os.path.join(os.getcwd(), "templates/main_template.cpp")) as main_template:
            line = main_template.readline()
            while line:
                content += line
                line = main_template.readline()
        
        content = content.replace("# Insert name", name)
        f.write(content)
    
    
def add_yaml_config(name: str):
    path_to_yaml_config = os.path.join(os.getcwd(),"Configs/" + name + "Config.yaml")
    if os.path.exists(path_to_yaml_config):
        print(f"\"{path_to_yaml_config}\" already exists, can't create")
        return
    content = ""
    
    with open(os.path.join(os.getcwd(), "templates/ConfigTemplate.yaml"), "r") as config_t:
        line = config_t.readline()
        while line:
            content += line
            line = config_t.readline()
            
    content = content.replace("# Insert name", name)
    with open(path_to_yaml_config, "x") as yaml_config:
        yaml_config.write(content)
            
        
                
def alter_cmakefile(name):
    path_to_cmakelist = os.path.join(os.getcwd(), "CMakeLists.txt")
    content = ""
    
    with open(path_to_cmakelist, "r") as get_content:
        line = get_content.readline()
        while line:
            content += line
            line = get_content.readline()
            
            if f"Void_{name}" in line:
                print(f"Found {name} in CMakefile, cancelling CMakefile modification")
                return
            
    file_macro_name = f"MAIN_{name.upper()}_SRC"
    content_to_add = f"file(GLOB {file_macro_name} \"mains/{name}Main.cpp\")\n"
    content_to_add += f"add_executable(Void_{name} $" + "{" + file_macro_name + "})\n"
    content_to_add += f"list(APPEND TARGETS Void_{name})\n\n"
    content_to_add += "# Replace here"
    
    content = content.replace("# Replace here", content_to_add)
            
    with open(path_to_cmakelist, "w") as write:
        write.write(content)
            
if __name__ == "__main__":
    
    arg_parser = ArgumentParser(
        description="Creates the folder structure for a given submodel",
        usage="create_submodels_structure.py <name>"
    )
    
    arg_parser.add_argument("name", type=str, help="The name of the submodel")
    args = arg_parser.parse_args()
    
    name = args.name
    name = str(name).capitalize()
    
    create_folder_structure(os.path.join(os.getcwd(), "include"), name, True)
    create_folder_structure(os.path.join(os.getcwd(), "src"), name, False)
    add_yaml_config(name)
    create_main(os.path.join(os.getcwd(), "mains"), name)
    alter_cmakefile(name)