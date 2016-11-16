from common import TestMetaWearBase
from mbientlab.metawear.sensor import SensorFusion

class TestSensorFusionConfig(TestMetaWearBase):
    acc_ranges = [SensorFusion.ACC_RANGE_2G, SensorFusion.ACC_RANGE_4G, SensorFusion.ACC_RANGE_8G, SensorFusion.ACC_RANGE_16G]
    gyro_ranges = [SensorFusion.GYRO_RANGE_2000DPS, SensorFusion.GYRO_RANGE_1000DPS, SensorFusion.GYRO_RANGE_500DPS, SensorFusion.GYRO_RANGE_250DPS]
    config_masks = [
        [0x10, 0x11, 0x12, 0x13],
        [0x20, 0x21, 0x22, 0x23],
        [0x30, 0x31, 0x32, 0x33],
        [0x40, 0x41, 0x42, 0x43]
    ]
    bmi160_acc_range_bitmask= [ 0b0011, 0b0101, 0b1000, 0b1100 ]
    bmi160_rot_range_bitmask= [ 0b000, 0b001, 0b010, 0b011, 0b100 ];


    def setUp(self):
        self.boardType = TestMetaWearBase.METAWEAR_MOTION_R_BOARD

        super().setUp()

    def queue_tests():
        tests= []

        for ar in TestSensorFusionConfig.acc_ranges:
            for gr in TestSensorFusionConfig.gyro_ranges:
                tests.append({
                    'acc_range': ar,
                    'gyro_range': gr
                })

        return tests

    def configure_algorithm(self, mode, acc, gyro):
        self.libmetawear.mbl_mw_sensor_fusion_set_mode(self.board, mode)
        self.libmetawear.mbl_mw_sensor_fusion_set_acc_range(self.board, acc)
        self.libmetawear.mbl_mw_sensor_fusion_set_gyro_range(self.board, gyro)
        self.libmetawear.mbl_mw_sensor_fusion_write_config(self.board)

    def test_configure_ndof(self):
        for test in TestSensorFusionConfig.queue_tests():
            with self.subTest():
                self.command_history = []
                self.configure_algorithm(SensorFusion.MODE_NDOF, test['acc_range'], test['gyro_range'])
                expected = [
                    [0x19, 0x02, SensorFusion.MODE_NDOF, TestSensorFusionConfig.config_masks[test['gyro_range']][test['acc_range']]],
                    [0x03, 0x03, 0x28, TestSensorFusionConfig.bmi160_acc_range_bitmask[test['acc_range']]],
                    [0x13, 0x03, 0x28, TestSensorFusionConfig.bmi160_rot_range_bitmask[test['gyro_range']]],
                    [0x15, 0x04, 0x04, 0x0e],
                    [0x15, 0x03, 0x6]
                ]
                self.assertEqual(self.command_history, expected)

    def test_configure_imu_plus(self):
        for test in TestSensorFusionConfig.queue_tests():
            with self.subTest():
                self.command_history = []
                self.configure_algorithm(SensorFusion.MODE_IMU_PLUS, test['acc_range'], test['gyro_range'])
                expected = [
                    [0x19, 0x02, SensorFusion.MODE_IMU_PLUS, TestSensorFusionConfig.config_masks[test['gyro_range']][test['acc_range']]],
                    [0x03, 0x03, 0x28, TestSensorFusionConfig.bmi160_acc_range_bitmask[test['acc_range']]],
                    [0x13, 0x03, 0x28, TestSensorFusionConfig.bmi160_rot_range_bitmask[test['gyro_range']]],
                ]
                self.assertEqual(self.command_history, expected)

    def test_configure_compass(self):
        for test in TestSensorFusionConfig.queue_tests():
            with self.subTest():
                self.command_history = []
                self.configure_algorithm(SensorFusion.MODE_COMPASS, test['acc_range'], test['gyro_range'])
                expected = [
                    [0x19, 0x02, SensorFusion.MODE_COMPASS, TestSensorFusionConfig.config_masks[test['gyro_range']][test['acc_range']]],
                    [0x03, 0x03, 0x26, TestSensorFusionConfig.bmi160_acc_range_bitmask[test['acc_range']]],
                    [0x15, 0x04, 0x04, 0x0e],
                    [0x15, 0x03, 0x6]
                ]
                self.assertEqual(self.command_history, expected)

    def test_configure_compass(self):
        for test in TestSensorFusionConfig.queue_tests():
            with self.subTest():
                self.command_history = []
                self.configure_algorithm(SensorFusion.MODE_M4G, test['acc_range'], test['gyro_range'])
                expected = [
                    [0x19, 0x02, SensorFusion.MODE_M4G, TestSensorFusionConfig.config_masks[test['gyro_range']][test['acc_range']]],
                    [0x03, 0x03, 0x27, TestSensorFusionConfig.bmi160_acc_range_bitmask[test['acc_range']]],
                    [0x15, 0x04, 0x04, 0x0e],
                    [0x15, 0x03, 0x6]
                ]
                self.assertEqual(self.command_history, expected)
