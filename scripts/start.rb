require 'orocos'
require 'readline'

include Orocos
Orocos.initialize


Orocos.run 'bus_schremote::Task' => 'bus_schremote',
	'spatial::Task' => 'spatial' do  
  
  bus_schremote = Orocos.name_service.get 'bus_schremote'
  spatial = Orocos.name_service.get 'spatial'
  
  bus_schremote.ip = '192.168.1.255' #'192.168.1.191'
  bus_schremote.mac = '00:04:A3:31:00:12'

  bus_schremote.uarts = [{:uart_module => 0, :mode => 0, :tx => 4, :rx => 0, :baud => 115200, :name => 'imu'}]
  #bus_schremote.imu_out.connect_to spatial.com_input
  bus_schremote.configure

  bus_schremote.imu_out.connect_to spatial.com_input
  spatial.configure

  bus_schremote.start
  spatial.start
  
  Readline::readline("Press ENTER to exit\n") do

  end
  

end

