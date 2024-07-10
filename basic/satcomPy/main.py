import threading
import time
from GroundStation import GroundStation
from Satellite import Satellite
import sys

def ground_station_thread(ground_station):
    time.sleep(1)  # 模拟一些延迟
    ground_station.send_data()

def main():
    language = 'en'  # 默认语言为英文
    if len(sys.argv) > 1:
        language = sys.argv[1]

    # 创建卫星对象
    satellite = Satellite(language)

    # 创建多个地面站对象并添加到卫星
    gs1 = GroundStation(satellite, language)
    gs2 = GroundStation(satellite, language)
    satellite.add_ground_station(gs1)
    satellite.add_ground_station(gs2)

    # 创建并启动地面站线程
    t1 = threading.Thread(target=ground_station_thread, args=(gs1,))
    t2 = threading.Thread(target=ground_station_thread, args=(gs2,))
    t1.start()
    t2.start()

    # 等待线程完成
    t1.join()
    t2.join()

if __name__ == "__main__":
    main()
