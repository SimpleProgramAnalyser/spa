#!/usr/bin/env python3
# Author: Han Liang Wee Eric
# GPL License

import os
import re
import sys
import glob

def check_compliance(compliance, sub_dir_files, path):
    sub_dirs = {}
    # Sort out the 3 files
    for f in sub_dir_files:
        # check which category is it in
        for cat in compliance:
            req = compliance[cat]
            if f==req:
                sub_dirs[cat] = os.path.join(path, f)

    missing_files = [ compliance[cat] for cat in set(compliance)-set(sub_dirs) ]
    if len(missing_files) != 0:
        print("[Failed] - Missing the following: {}".format(','.join(missing_files)))
        sys.exit()
    return sub_dirs

def is_empty(dir_name, base_path):
    if dir_name in os.listdir(base_path):
        if len(os.listdir(os.path.join(base_path, dir_name))) != 0:
            print("[Failed] - Directory {} should be empty, but its not.".format(dir_name))
            sys.exit()
    print("[Passed] - Directory {} is clean.".format(dir_name))

print("This script will check for basic compliance with the submission requirements.")
print("Disclaimer: you are still responsible for your submission, this check is by")
print("no means complete.")
print("-"*80)

files = [f for f in os.listdir('.') if re.match(r'Team[0-9][0-9]', f)]
if len(files) > 1:
    print("[Failed] - This script cannot be executed with multiple teams' folders.")
    sys.exit()
    
if len(files) == 0:
    print("[Failed] - This script cannot find the Team folder, please make sure it exist and named correctly.")
    sys.exit()

# Single Team folder
team_folder = files[0]
team_no_str = team_folder[4:]
team_no = int(team_no_str)
if team_no < 1:
    print("[Failed] - Team number must be valid.")
    sys.exit()
    
print("[Passed] - Single team folder detected, Team number is {}.".format(team_no) )

sub_dir_files = [ f for f in os.listdir(os.path.join(team_folder)) ]
if len(sub_dir_files) != 3:
    print("[Failed] - Must only contain 3 items: Tests dir, Code dir and report pdf")
    sys.exit()

sub_dirs_compliance = {
    "code" : "Code{}".format(team_no_str),
    "report" : "Team{}.pdf".format(team_no_str),
    "tests" : "Tests{}".format(team_no_str)
}
sub_dirs = check_compliance(sub_dirs_compliance, sub_dir_files, os.path.join(team_folder)) 

base_code_path = sub_dirs['code']
base_tests_path = sub_dirs['tests']
for f in sub_dirs:
    print("[Passed] - Found correctly formatted {}: {}".format(f, sub_dirs[f]))

# now we check the code compliance
code_sub_dirs_compliance = {
    "readme" : "Readme.txt",
    "cmakelists" : "CMakeLists.txt",
    "src" : "src",
    "contact" : "Contact.csv"
}
code_sub_dirs = check_compliance(code_sub_dirs_compliance, os.listdir(os.path.join(base_code_path)), base_code_path)
for f in code_sub_dirs:
    print("[Passed] - Found correctly formatted {}: {}".format(f, code_sub_dirs[f]))

# Ensure that debug and release are empty
is_empty("Release", base_code_path)
is_empty("Debug", base_code_path)

# we now check if the tests directory is properly organized
test_sub_dirs_regex = {
    "queries" : r"[a-zA-Z0-9\-]+_queries.txt",
    "source" : r"[a-zA-Z0-9\-]+_source.txt"
}
test_sub_dirs = {
    "queries" : set(),
    "source" : set()
}
for f in os.listdir(base_tests_path):
    for cat in test_sub_dirs_regex:
        regex = test_sub_dirs_regex[cat]
        if re.match(regex, f):
            test_sub_dirs[cat].add(f)

# We attempt to validate the files of each test case, which each will have 2 parts: source and queries
if len(test_sub_dirs["queries"]) != len(test_sub_dirs["source"]):
    print("[Failed] Each testcase should have a source and query pair.")

for query in test_sub_dirs["queries"]:
    test_name = query[:-12]
    if not "{}_source.txt".format(test_name) in test_sub_dirs["source"]:
        print("[Failed] - Test {} does not have both source and query file.".format(test_name))
        sys.exit()
    else:
        print("[Passed] - Test {} has both source and query file.".format(test_name))

# Now we check for hidden files
for root, dirnames, filenames in os.walk("."):
    for filename in filenames:
        if filename.startswith("."): #  and not ".vs" in root
            print("[Failed] - Hidden file {} should not exist.".format(os.path.join(root, filename)))

    for dir in dirnames:
        if dir.startswith("."): #  and dir != ".vs"
            print("[Failed] - Hidden dir {} should not exist.".format(os.path.join(root, dir)))

# Now we have established correct subdirs
print("="*80)
print("Project conforms to basic submission requirements.")
