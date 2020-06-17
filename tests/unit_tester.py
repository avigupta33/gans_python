from typing import *

# Allow for importation of modules from parent folders
import os,sys,inspect
current_dir = os.path.dirname(os.path.abspath(inspect.getfile(inspect.currentframe())))
parent_dir = os.path.dirname(current_dir)
sys.path.insert(0, parent_dir)

# A unit testing framework 

def runTests(title: str, tests: List[Callable[[], bool]]) -> None:
    '''Evaluate test cases and print results'''
    num_passed = 0
    # Has to be expanded to print fails
    for test in tests:
        try:
            if test():
                num_passed += 1
            else:
                print(f"Failed on {test.__name__}")
        except Exception as e:
            print(f"Uncaught exception on {test.__name__}: {e}")
    print(f"{title} passed {num_passed}/{len(tests)} cases")
