import uuid
import threading

class GroundStation:
    def __init__(self, satellite, language='en'):
        self.satellite = satellite
        self.uuid = str(uuid.uuid4())
        self.language = language
        self.lock = threading.Lock()
        if language == 'cn':
            print(f"创建 GroundStation 对象，UUID 为: {self.uuid}")
        else:
            print(f"GroundStation created with UUID: {self.uuid}")

    def send_data(self):
        if self.language == 'cn':
            print(f"地面站 {self.uuid} 正在向卫星发送数据...")
        else:
            print(f"GroundStation {self.uuid} sending data to satellite...")
        self.satellite.notify_ground_stations()

    def receive_data(self):
        if self.language == 'cn':
            print(f"地面站 {self.uuid} 接收到来自卫星的数据。")
        else:
            print(f"GroundStation {self.uuid} received data from satellite.")
