#!/usr/bin/python3
import subprocess
import inspect
import os

xxd_test_name = "./xxd-TESTING"
xxd_original_name = "xxd"

def _test_helloworld():
    if(True):
        return True;

def make_offsets(off_in_name, off_out_name):
    with open(off_in_name, 'r') as infile:
        with open(off_out_name, 'w') as outfile:
            for line in infile:
                #print(f"Original line: {repr(line)}")  # Debug: Show full line including invisible characters
                #print(f"Sliced part: {repr(line[:9])}")  # Debug: Show the sliced part
                outfile.write(line[:9] + '\n')
    os.remove(off_in_name)

def _test_diff_offsets():
    t_off_in_name = "t_xxd"
    t_off_out_name2 = "t_xxd_only_off"
    o_off_in_name = "o_xxd"
    o_off_out_name2 = "o_xxd_only_off"
    testing_result = subprocess.run([xxd_test_name, xxd_test_name, t_off_in_name])
    original_result = subprocess.run([xxd_original_name, xxd_test_name, o_off_in_name])
    make_offsets(t_off_in_name, t_off_out_name2);    
    make_offsets(o_off_in_name, o_off_out_name2);
    diff_result = subprocess.run(["diff", "-sq", f"{o_off_out_name2}", f"{t_off_out_name2}"], capture_output=True, text=True)
    compare = diff_result.stdout.strip()
    if compare[-1] == 'l':
        os.remove(t_off_out_name2)
        os.remove(o_off_out_name2)
        return True

    if compare[-1] == 'r':
        os.remove(t_off_out_name2)
        os.remove(o_off_out_name2)
        return False

def run_tests():
    current_module = globals()
    test_functions = {
        name: func for name, func in current_module.items()
        if callable(func) and name.startswith("_test_")
    }

    passed = 0
    total = len(test_functions)

    for name, test_func in sorted(test_functions.items()):
        try:
            result = test_func()
            if result:
                print(f"[PASS] {name}")
                passed += 1
            else:
                print(f"[FAIL] {name}")
        except Exception as e:
            print(f"[ERROR] {name}: {e}")

    print(f"{passed}/{total} tests passed.")

# === MAIN ===

if __name__ == "__main__":
    os.environ['NO_COLOR'] = ''
    run_tests()
    
