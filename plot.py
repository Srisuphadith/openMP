import pandas as pd
import matplotlib.pyplot as plt

# รายชื่อไฟล์ CSV (แก้ตามของจริง)
files = [
    "benchmark_threads_2.csv",
    "benchmark_threads_4.csv",
    "benchmark_threads_8.csv"
]

threads = []
avg_times = []

for fname in files:
    df = pd.read_csv(fname)

    # จำนวน thread (เหมือนกันทั้งไฟล์)
    n_threads = df["threads"].iloc[0]

    # ค่าเฉลี่ยเวลา
    avg_time = df["time_sec"].mean()

    threads.append(n_threads)
    avg_times.append(avg_time)

# เรียงตามจำนวน thread (กันกรณีไฟล์ไม่เรียง)
threads, avg_times = zip(*sorted(zip(threads, avg_times)))

# plot
plt.plot(threads, avg_times, marker="o")
plt.xlabel("Number of Threads")
plt.ylabel("Average Time (seconds)")
plt.title("Average Execution Time vs Threads")
plt.grid(True)
plt.show()