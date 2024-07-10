import threading

class Satellite:
    def __init__(self, language='en'):
        self.ground_stations = []
        self.lock = threading.Lock()
        self.language = language
        if language == 'cn':
            print("创建 Satellite 对象，使用列表来存储地面站。")
        else:
            print("Satellite created, using list to store ground stations.")

    def add_ground_station(self, ground_station):
        with self.lock:
            self.ground_stations.append(ground_station)
            if self.language == 'cn':
                print(f"UUID 为: {ground_station.uuid} 的地面站已添加。")
            else:
                print(f"Ground station with UUID: {ground_station.uuid} added.")

    def notify_ground_stations(self):
        with self.lock:
            if self.language == 'cn':
                print("通知所有地面站...")
            else:
                print("Notifying ground stations...")
            for gs in self.ground_stations:
                if gs:
                    if self.language == 'cn':
                        print(f"通知 UUID 为: {gs.uuid} 的地面站。")
                    else:
                        print(f"Notifying ground station with UUID: {gs.uuid}.")
                    gs.receive_data()
