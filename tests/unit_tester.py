from typing import *

# Allow for importation of modules from parent folders
import os,sys,inspect
current_dir = os.path.dirname(os.path.abspath(inspect.getfile(inspect.currentframe())))
parent_dir = os.path.dirname(current_dir)
sys.path.insert(0, parent_dir)


def runTestsFromEnv(title: str, local: Dict[str, Any], prefix: str="test") -> None:
    '''Evaluates test cases found in the environment'''
    tests = [local[key] for key in local.keys() if key.startswith(prefix)]
    runTests(title, tests)


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
            print(f"Uncaught exception on {test.__name__}:\n  {type(e).__name__}: {e}")
    print(f"{title} passed {num_passed}/{len(tests)} cases")


def raises(func: Callable[[], Any], err_type: Exception, message: str) -> bool:
    '''Determine if a function raises a specific exception'''
    try:
        func()
        # Expected an exception but none were raised
        return False
    except err_type as e:
        if str(e) == message:
            # Expected exception was raised
            return True
        # Exception message doesn't match
        raise e
    # Exception of unexpected type is raised


def isPassing(expected: Any, received: Any) -> bool:
    '''Syntactic sugar to check equivalency'''
    return expected == received
