# SMART GREENHOUSE IoT (Dự án nhà kính trồng cây thông minh)
## Đặt vấn đề

Trong bối cảnh nông nghiệp hiện đại, việc sử dụng công nghệ để cải thiện năng suất và chất lượng cây trồng đang trở thành xu hướng phổ biến. Đặc biệt, với sự phát triển của công nghệ Internet of Things (IoT), việc giám sát và điều khiển môi trường trồng cây từ xa trở nên dễ dàng và hiệu quả hơn bao giờ hết. Các hệ thống nhà kính (greenhouses) được coi là một giải pháp tối ưu để trồng cây trong môi trường có kiểm soát, giúp tối ưu hóa các yếu tố như ánh sáng, nhiệt độ, độ ẩm và độ ẩm của đất.

Tuy nhiên, việc vận hành các hệ thống nhà kính truyền thống vẫn đòi hỏi sự can thiệp và giám sát thường xuyên của con người. Điều này không chỉ tốn thời gian và công sức mà còn có thể dẫn đến sự thiếu chính xác trong việc kiểm soát môi trường, ảnh hưởng đến sự phát triển của cây trồng.
## Tổng quan 
Xuất phát từ dự án giữa kỳ là hệ thống tưới nước thông minh đơn giản, em phát triển thêm 1 số module để thực hiện project này.

Dự án Hệ Thống Nhà Kính Trồng Cây Thông Minh sử dụng công nghệ IoT và các cảm biến để giám sát và điều khiển môi trường trong nhà kính, giúp tối ưu hóa quá trình trồng cây. Hệ thống này sử dụng ESP32, cảm biến độ ẩm đất, cảm biến nhiệt độ và độ ẩm không khí,mắt thu hồng ngoại, và hệ thống tưới tự động để duy trì điều kiện môi trường lý tưởng cho cây trồng.

Hệ thống có khả năng điều chỉnh môi trường trong nhà kính bằng cách bật/tắt các thiết bị như đèn chiếu sáng, và hệ thống tưới cây tự động. Các dữ liệu cảm biến sẽ được thu thập và truyền tải đến MQTT broker, giúp người dùng giám sát và điều khiển từ xa thông qua ứng dụng hoặc giao diện web.
## Sơ đồ khối

![image](https://github.com/user-attachments/assets/37a91608-5aa0-425a-b03c-b4b76c77c6b8)
## Chi tiết thành phần dự án
  1. Giám Sát Độ Ẩm Đất

Sử dụng cảm biến độ ẩm đất để đo độ ẩm trong đất và tự động bật/tắt hệ thống tưới cây khi cần thiết. Nếu độ ẩm đất thấp hơn ngưỡng quy định, hệ thống sẽ tự động bật máy bơm để tưới cây.

  2. Giám Sát Nhiệt Độ, Độ Ẩm Không Khí và Chất Lượng Không Khí

Sử dụng cảm biến DHT11 để đo nhiệt độ và độ ẩm không khí trong nhà kính.

  3. Điều Khiển Ánh Sáng

Sử dụng mắt thu hồng ngoại kết hợp với IR Remote để điều khiển bật tắt đèn led trong phòng.

  4. Tưới Tự Động

Kết hợp cảm biến độ ẩm đất và rơ le để điều khiển máy bơm nước tự động bật/tắt khi độ ẩm đất giảm xuống dưới mức cần thiết. Điều này giúp tiết kiệm nước và đảm bảo cây trồng luôn được tưới đúng lượng.

  5. Giám Sát và Điều Khiển Từ Xa

Sử dụng MQTT để gửi dữ liệu cảm biến lên MQTT broker. Người dùng có thể giám sát và điều khiển môi trường nhà kính từ xa thông qua một giao diện web hoặc ứng dụng di động, giúp tối ưu hóa công việc trồng cây.

  6. Chế Độ Thủ Công và Tự Động

Hệ thống có thể chuyển đổi giữa chế độ thủ công và tự động để người dùng có thể can thiệp vào quá trình điều khiển nếu cần. Trong chế độ tự động, hệ thống sẽ hoạt động theo các ngưỡng đã được định nghĩa, còn trong chế độ thủ công, người dùng có thể điều khiển trực tiếp thông qua các lệnh MQTT hoặc giao diện.

### Chi tiết khối linh kiện sử dụng

1.Khối điều khiển đèn led tại cửa ra vào: Ý tưởng của em là sẽ thiết kế một hệ thống chiếu sáng sử dụng cảm biến PIR HC-SR501 để phát hiện chuyển động,nếu có người đến gần thì đèn sẽ được sáng lên.Điều này giúp ích cho người chăm sóc trong quá trình đi lại khi trời quá tối.

https://github.com/user-attachments/assets/8d3f8475-a661-4000-b03a-2f9da0375198

2.Khối điều khiển đèn trong nhà : Ở đây em thiết kế một đèn led bên cạnh đó là 1 mắt thu hồng ngoại,người dùng có thể sử dụng 1 thiết bị là điều khiển hồng ngoại(IRremote).Với việc sử dụng thiết bị này,người chăm sóc sẽ không cần phải đi đến tận nơi để bật tắt thiết bị đèn.Thiết bị này có thể cải tiến hơn nữa nếu như được tích hợp thêm các module như quạt gió...

Ở đây,người dùng cũng có thêm 1 lựa chọn đó là có thể sử dụng node-red trên các thiết bị có kết nối internet,chỉ với thao tác bật tắt đơn giản.

https://github.com/user-attachments/assets/b283613f-e6bb-4fa2-a68b-9f39c738c325

3.Khối thực thi tưới nước tự động : Sử dụng 1 module cảm biến độ ẩm đất và relay để thực hiện việc bơm nước.Quy trình sẽ được thực hiện dựa trên việc đo độ ẩm của đất.

Chế độ tự động : Dựa trên thiết lập của em trong file code,tùy mục đích với từng loại cây trồng mà sẽ có thiết lập khác nhau.

https://github.com/user-attachments/assets/22f7c048-31cb-4884-923a-935ed841dc4b

Chế độ thủ công: Khi chuyển sang chế độ này,người dùng có thể bật tắt thiết bị relay bơm nước tùy ý bất chấp độ ẩm hiện tại là bao nhiêu.

https://github.com/user-attachments/assets/f4256e30-2e0f-4ea0-aacf-23c05331df51

4.Khối đo cảm biến nhiệt độ độ ẩm và chất lượng không khí: Ở đây em sử dụng cảm biến DHT11 và MQ135

Đây là chi tiết file cấu hình trên dashboard node-red của em
![image](https://github.com/user-attachments/assets/368d0556-9979-46f4-9f2b-e0e5adfab830)
## Kết Luận
Hệ thống nhà kính thông minh giúp tự động hóa quá trình chăm sóc cây trồng, tiết kiệm năng lượng và tài nguyên như nước và điện. Các cảm biến giám sát các yếu tố môi trường trong nhà kính và cung cấp dữ liệu quan trọng, từ đó hệ thống có thể tự động điều chỉnh các thiết bị (máy bơm, quạt, đèn) để tạo ra môi trường lý tưởng cho sự phát triển của cây. Dự án này có thể mở rộng thêm các tính năng như giám sát thêm các yếu tố môi trường, kết nối với các nền tảng tự động hóa như Home Assistant, hoặc thêm các chức năng điều khiển qua voice assistant.

Vì dự án này vẫn còn khá khiêm tốn, trong tương lai em đang muốn tích hợp thêm 1 số tính năng mới như radar để thực hiện bắt côn trùng hay là điều hòa không khí ...
