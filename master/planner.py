import os
import tkinter as tk
from tkinter import messagebox
from PIL import Image, ImageDraw

class DrawingBoard:
    def __init__(self, root):
        self.root = root
        self.root.title("KHOAI Master - Bang Ve CNC")
        
        self.canvas_width = 500
        self.canvas_height = 500
        
        # Khoi tao giao dien ve
        self.canvas = tk.Canvas(self.root, width=self.canvas_width, height=self.canvas_height, bg="white")
        self.canvas.pack(pady=10)
        
        # Khoi tao anh ao tren RAM de luu thanh file PNG
        self.image = Image.new("RGB", (self.canvas_width, self.canvas_height), "white")
        self.draw = ImageDraw.Draw(self.image)
        
        # Bien luu tru toa do
        self.coordinates = []
        self.last_x = None
        self.last_y = None
        
        # Bat su kien chuot
        self.canvas.bind("<Button-1>", self.start_draw)
        self.canvas.bind("<B1-Motion>", self.draw_line)
        self.canvas.bind("<ButtonRelease-1>", self.stop_draw)
        
        # Khu vuc dieu khien
        btn_frame = tk.Frame(self.root)
        btn_frame.pack(pady=5)
        
        self.btn_clear = tk.Button(btn_frame, text="Xoa", width=10, command=self.clear_canvas)
        self.btn_clear.pack(side=tk.LEFT, padx=10)
        
        self.btn_done = tk.Button(btn_frame, text="Xong", width=10, command=self.save_data)
        self.btn_done.pack(side=tk.LEFT, padx=10)
        
        # Tao thu muc chua data neu chua ton tai
        if not os.path.exists("data"):
            os.makedirs("data")

    def start_draw(self, event):
        self.last_x = event.x
        self.last_y = event.y
        self.coordinates.append((self.last_x, self.last_y))

    def draw_line(self, event):
        if self.last_x and self.last_y:
            # Hien thi net ve tren man hinh
            self.canvas.create_line(self.last_x, self.last_y, event.x, event.y, fill="black", width=2, capstyle=tk.ROUND)
            
            # Ghi net ve vao anh ao Pillow
            self.draw.line([self.last_x, self.last_y, event.x, event.y], fill="black", width=2)
            
            # Cap nhat toa do
            self.last_x = event.x
            self.last_y = event.y
            self.coordinates.append((self.last_x, self.last_y))

    def stop_draw(self, event):
        # Ngat lien ket diem khi nha chuot
        self.last_x = None
        self.last_y = None
        # Them mot diem ngat dong de C++ biet ban nhac but
        self.coordinates.append((-1, -1)) 

    def clear_canvas(self):
        # Xoa du lieu man hinh
        self.canvas.delete("all")
        
        # Reset anh ao tren RAM
        self.image = Image.new("RGB", (self.canvas_width, self.canvas_height), "white")
        self.draw = ImageDraw.Draw(self.image)
        
        # Reset danh sach toa do
        self.coordinates.clear()
        messagebox.showinfo("Thong bao", "Da xoa bang ve!")

    def save_data(self):
        if not self.coordinates:
            messagebox.showwarning("Canh bao", "Bang ve dang trong!")
            return
            
        try:
            # Luu anh preview
            self.image.save("data/image.png")
            
            # Tinh toan duong dan luu file PathData.h vao thu muc C++
            # __file__ la vi tri cua planner.py (nam trong thu muc master)
            current_dir = os.path.dirname(os.path.abspath(__file__))
            
            # Lui lai 1 cap de ve thu muc root BTL, sau do vao lib/PathPlanner
            project_root = os.path.dirname(current_dir)
            target_dir = os.path.join(project_root, "lib", "PathPlanner")
            os.makedirs(target_dir, exist_ok=True)
            
            file_path = os.path.join(target_dir, "PathData.h")
            
            # Ghi file header C++
            with open(file_path, "w") as f:
                f.write("#ifndef PATHDATA_H\n#define PATHDATA_H\n\n")
                
                # Tu dong nhan dien moi truong de chon vi tri luu bo nho
                f.write("#ifdef ARDUINO\n")
                f.write("    #include <avr/pgmspace.h>\n")
                f.write("    #define DAT_MEM PROGMEM\n")
                f.write("#else\n")
                f.write("    #define DAT_MEM\n")
                f.write("#endif\n\n")
                
                # Loc bo cac diem ngat net (-1, -1)
                valid_coords = [pt for pt in self.coordinates if pt[0] != -1]
                
                f.write(f"const int PATH_POINT_COUNT = {len(valid_coords)};\n\n")
                
                # Khai bao mang kem macro DAT_MEM
                f.write("const float pathX[] DAT_MEM = {")
                f.write(", ".join(str(pt[0]) for pt in valid_coords))
                f.write("};\n\n")
                
                f.write("const float pathY[] DAT_MEM = {")
                f.write(", ".join(str(pt[1]) for pt in valid_coords))
                f.write("};\n\n")
                
                f.write("#endif\n")
                
            messagebox.showinfo("Thanh cong", f"Da sinh code C++ tai:\n{file_path}")
            
        except Exception as e:
            messagebox.showerror("Loi", f"Xuat file that bai: {e}")

def main():
    root = tk.Tk()
    app = DrawingBoard(root)
    root.mainloop()

if __name__ == "__main__":
    main()