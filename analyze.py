import os
import re

# Function to parse the perf output file and extract metrics
def parse_perf_output(filename):
    metrics = {
        'cycles': 0,
        'instructions': 0,
        'cache-misses': 0,
        'branches': 0,
        'branch-misses': 0
    }
    with open(filename, 'r') as file:
        for line in file:
            if 'cycles' in line:
                metrics['cycles'] = int(re.search(r'([\d,]+)', line).group(1).replace(',', ''))
            elif 'instructions' in line:
                metrics['instructions'] = int(re.search(r'([\d,]+)', line).group(1).replace(',', ''))
            elif 'cache-misses' in line:
                metrics['cache-misses'] = int(re.search(r'([\d,]+)', line).group(1).replace(',', ''))
            elif 'branches' in line:
                metrics['branches'] = int(re.search(r'([\d,]+)', line).group(1).replace(',', ''))
            elif 'branch-misses' in line:
                metrics['branch-misses'] = int(re.search(r'([\d,]+)', line).group(1).replace(',', ''))
    return metrics

# Function to calculate IPC (Instructions Per Cycle)
def calculate_ipc(instructions, cycles):
    return instructions / cycles if cycles != 0 else 0

# Function to calculate the branch miss rate
def calculate_branch_miss_rate(branch_misses, branches):
    return branch_misses / branches if branches != 0 else 0

# Analyze the perf output for each thread and determine the best logical core
def analyze_perf_data():
    num_cores = 4  # Adjust based on system cores
    core_assignments = {}
    thread_performance = []

    # Gather performance data for each thread (assumes files named 'perf_thread_<threadIdx>.txt')
    for threadIdx in range(3):  # Adjust for actual number of threads
        filename = f'perf_thread_{threadIdx}.txt'
        if not os.path.exists(filename):
            print(f"Error: {filename} not found.")
            continue

        metrics = parse_perf_output(filename)
        ipc = calculate_ipc(metrics['instructions'], metrics['cycles'])
        cache_misses = metrics['cache-misses']
        branch_miss_rate = calculate_branch_miss_rate(metrics['branch-misses'], metrics['branches'])

        # Store each thread's data for sorting and assignment
        thread_performance.append({
            'threadIdx': threadIdx,
            'ipc': ipc,
            'cache_misses': cache_misses,
            'branch_miss_rate': branch_miss_rate
        })

    # Sort threads by performance metrics for optimal assignment
    # Priority: High cache misses, then low IPC, then high branch miss rate
    thread_performance.sort(key=lambda x: (-x['cache_misses'], x['ipc'], -x['branch_miss_rate']))

    # Assign each thread to a core in a round-robin or based on resource intensity
    assigned_cores = [0] * num_cores  # To track threads assigned per core

    for thread_data in thread_performance:
        # Find the least loaded core for assignment
        best_core = assigned_cores.index(min(assigned_cores))
        core_assignments[thread_data['threadIdx']] = best_core
        assigned_cores[best_core] += 1  # Increment load for that core

    # Write the core assignments to core_optimization.txt
    with open("core_optimization.txt", "w") as output_file:
        for thread_id, core in core_assignments.items():
            output_file.write(f"{thread_id}:{core}\n")
        output_file.write("\n")

if __name__ == '__main__':
    analyze_perf_data()
