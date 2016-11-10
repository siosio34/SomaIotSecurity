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


    client.close

  end

  def setting_send

    @edit_admin_pass = params[:edit_admin_pass]
    @edit_local_ssid = params[:edit_local_ssid]
    @edit_local_pass = params[:edit_local_pass]
    @edit_guest_ssid = params[:edit_guest_ssid]
    @edit_guest_pass = params[:edit_guest_pass]

    puts @edit_admin_pass
    puts @edit_local_ssid
    puts @edit_local_pass
    puts @edit_guest_ssid
    puts @edit_guest_pass


  end


def chlidren

  end

  def otp

  end

  def monitoring

  end

end
