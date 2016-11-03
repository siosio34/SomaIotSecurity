require 'socket'
require 'json'

class UtilController < ApplicationController

  def setting

    passjson = {:page_name => 'request' , :request_id => 'ssid'}
    client = TCPSocket.new 'localhost', 9090
    client.puts JSON.generate(passjson)

    response = client.read
    strip_json = response.strip
    puts strip_json

    my_hash = JSON.parse(strip_json)

    @local_ssid = my_hash["local_ssid"]
    @guest_ssid = my_hash["guest_ssid"]
    @otp_enable = my_hash["otp_enable"]

    puts @local_ssid
    puts @guest_ssid
    puts @otp_enable

    client.close

  end

  def chlidren

  end

  def otp

  end

  def monitoring

  end

end
