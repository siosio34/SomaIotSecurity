require 'rubygems'
require 'socket'
require 'json'
require 'csv'

class LoginController < ApplicationController

  def index

  end

  def auth
    @pass = params[:password]
    passjson = {:page_name => 'login', :admin_pw => @pass}
    client = TCPSocket.new 'localhost', 9090

    client.puts JSON.generate(passjson)
    response = client.read
    strip_json = response.strip
    puts strip_json
    my_hash = JSON.parse(strip_json)
    if my_hash["verify"] == 'true'
      render 'welcome/index'
    else
      render 'login/index'

    client.puts JSON.generate(passjson)
    client.close

    end
  end



end
