import numpy as np
import matplotlib.pyplot as plt
from matplotlib.widgets import Slider

# PID 参数
Kf = 0.6
Kp = 0.8
Ki = 0.1
Kd = 0.0
MAX_Error = 200.0
MAX_Integral = 500.0
MAX_OutPut = 5000.0
dt = 0.005

def limit(x, lower, upper):
    return max(min(x, upper), lower)

def pid_calculate(expect, feedback, integral):
    """
    支持负数 feedback/expect 的 PID 计算
    """
    error = expect - feedback
    # 允许负误差积分累加，不再强制 >=0
    points_ratio = 1 - (error**2)/(MAX_Error**2)
    points_ratio = limit(points_ratio, -1, 1)  # 限制在 [-1,1] 防爆积分
    integral += points_ratio * error * dt
    integral = limit(integral, -MAX_Integral, MAX_Integral)
    differential = 0
    OutPut = Kf*expect + Kp*error + Ki*integral - Kd*differential
    OutPut = limit(OutPut, -MAX_OutPut, MAX_OutPut)
    return OutPut, integral

# 初始化图形
expect_init = 1000
feedback_values = np.linspace(-8000, 8000, 500)
integral = 0.0
outputs = []
for fb in feedback_values:
    out, integral = pid_calculate(expect_init, fb, integral)
    outputs.append(out)

fig, ax = plt.subplots()
plt.subplots_adjust(bottom=0.25)
l, = plt.plot(feedback_values, outputs, lw=2, color='blue')
target_line = ax.axvline(expect_init, color='red', linestyle='--', label='Target value')

ax.set_xlabel('Feedback')
ax.set_ylabel('PID Output')
ax.set_title('PID Output vs Feedback')
ax.set_xlim(-8000, 8000)
ax.set_ylim(-5000, 5000)
ax.grid(True)
ax.legend()

# 添加滑块
ax_expect = plt.axes([0.2, 0.1, 0.65, 0.03])
slider_expect = Slider(ax_expect, 'Expect', 0, 5000, valinit=expect_init)

def update(val):
    expect = slider_expect.val
    feedback_values = np.linspace(-8000, 8000, 500)
    integral = 0.0
    outputs = []
    for fb in feedback_values:
        out, integral = pid_calculate(expect, fb, integral)
        outputs.append(out)
    l.set_xdata(feedback_values)
    l.set_ydata(outputs)
    target_line.set_xdata([expect, expect])
    # 坐标轴固定
    l.axes.set_xlim(-8000, 8000)
    l.axes.set_ylim(-5000, 5000)
    fig.canvas.draw_idle()

slider_expect.on_changed(update)
plt.show()
