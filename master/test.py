import turtle
import os

def load_commands_from_txt(file_path):
    commands = []
    try:
        with open(file_path, 'r', encoding='utf-8') as f:
            for line in f:
                # Xoa khoang trang va ky tu xuong dong o 2 dau
                line = line.strip()
                
                # Bo qua cac dong trong
                if not line:
                    continue
                    
                parts = line.split()
                # Kiem tra dong co du 2 thanh phan: Lenh va Gia tri
                if len(parts) == 2:
                    # Chuan hoa chu in hoa de de so sanh
                    cmd = parts[0].upper()
                    try:
                        value = float(parts[1])
                        commands.append((cmd, value))
                    except ValueError:
                        print(f"Loi ep kieu so o dong: {line}")
                else:
                    print(f"Dong sai dinh dang (bo qua): {line}")
    except FileNotFoundError:
        print("Khong tim thay file result.txt")
        
    return commands

def simulate():
    # Tro dung vao file result.txt nam trong thu muc data
    current_dir = os.path.dirname(os.path.abspath(__file__))
    txt_path = os.path.join(current_dir, "data", "result.txt")
    
    commands = load_commands_from_txt(txt_path)
    if not commands:
        print("Khong co du lieu de mo phong.")
        return

    screen = turtle.Screen()
    t = turtle.Turtle()
    t.speed(0) 
    
    scale = 5
    springback_factor = 1.05
    
    for cmd, value in commands:
        if cmd == "FEED":
            t.forward(value * scale)
        elif cmd == "BEND":
            # Dich nguoc goc be tu toa do servo thuc te ve goc hinh hoc
            deflection_angle = (value - 90.0) / springback_factor
            t.left(deflection_angle)
            
    print("Trang thai hoan tat")
    screen.mainloop()

if __name__ == "__main__":
    simulate()