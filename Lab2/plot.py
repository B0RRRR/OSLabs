import pandas as pd
import matplotlib.pyplot as plt

# === Чтение CSV ===
# Ожидается формат: threads,time
df = pd.read_csv("results.csv")

# Сортируем по количеству потоков на случай, если порядок нарушен
df = df.sort_values(by="threads")

# === 1. График времени выполнения ===
plt.figure(figsize=(8, 5))
plt.plot(df["threads"], df["time"], marker="o", label="Время выполнения")
for i, t in enumerate(df["time"]):
    plt.text(df["threads"].iloc[i], t, f"{t:.4f}", ha='left', va='bottom', fontsize=8)

plt.title("Зависимость времени выполнения от количества потоков")
plt.xlabel("Количество потоков")
plt.ylabel("Время (секунды)")
plt.grid(True)
plt.legend()
plt.tight_layout()
plt.savefig("results_time.png", dpi=300)
plt.close()

# === 2. График ускорения и эффективности ===
t1 = df["time"].iloc[0]  # время при 1 потоке
df["speedup"] = t1 / df["time"]
df["efficiency"] = df["speedup"] / df["threads"]

# График ускорения
plt.figure(figsize=(8, 5))
plt.plot(df["threads"], df["speedup"], marker="o", color="green", label="Ускорение")
plt.plot(df["threads"], df["threads"], "--", color="gray", label="Идеальное ускорение")
for i, s in enumerate(df["speedup"]):
    plt.text(df["threads"].iloc[i], s, f"{s:.2f}", ha='left', va='bottom', fontsize=8)

plt.title("График ускорения (Speedup)")
plt.xlabel("Количество потоков")
plt.ylabel("Ускорение")
plt.grid(True)
plt.legend()
plt.tight_layout()
plt.savefig("results_speedup.png", dpi=300)
plt.close()

# График эффективности
plt.figure(figsize=(8, 5))
plt.plot(df["threads"], df["efficiency"], marker="o", color="orange", label="Эффективность")
for i, e in enumerate(df["efficiency"]):
    plt.text(df["threads"].iloc[i], e, f"{e*100:.1f}%", ha='left', va='bottom', fontsize=8)

plt.title("График эффективности (Efficiency)")
plt.xlabel("Количество потоков")
plt.ylabel("Эффективность")
plt.grid(True)
plt.legend()
plt.tight_layout()
plt.savefig("results_efficiency.png", dpi=300)
plt.close()

print("✅ Графики успешно сохранены: results_time.png, results_speedup.png, results_efficiency.png")
