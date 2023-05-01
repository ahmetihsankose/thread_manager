import numpy as np

filename = "test.txt"

thread_ids = set()

with open(filename, "r") as f:
    for line in f:
        if line.strip():
            thread_id = int(line.strip().split(", ")[0].split(": ")[1])
            thread_ids.add(thread_id)

num_threads = len(thread_ids)
print(f"Number of threads: {num_threads}")

stats = {i: {"total_tasks": 0, "total_time": 0} for i in range(num_threads)}

with open(filename, "r") as f:
    for line in f:
        if line.strip():
            thread_id, total_tasks, avg_value = line.strip().split(", ")
            thread_id = int(thread_id.split(": ")[1])
            total_tasks = int(total_tasks.split(": ")[1])
            avg_value = int(avg_value.split(": ")[1].replace("ns", ""))
            
            stats[thread_id]["total_tasks"] += total_tasks
            stats[thread_id]["total_time"] += total_tasks * avg_value

for thread_id in stats:
    total_tasks = stats[thread_id]["total_tasks"]
    total_time = stats[thread_id]["total_time"]
    avg_time = total_time / total_tasks if total_tasks > 0 else np.nan
    print(f"Thread {thread_id}: Total tasks = {total_tasks}, Average value = {avg_time:.2f} ns")

total_tasks = sum([stats[thread_id]["total_tasks"] for thread_id in stats])
total_time = sum([stats[thread_id]["total_time"] for thread_id in stats])
avg_time = total_time / total_tasks if total_tasks > 0 else np.nan
print(f"\nAll threads: Total tasks = {total_tasks}, Average value = {avg_time:.2f} ns")
