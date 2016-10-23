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
    puts JSON.generate(passjson)
    puts JSON.generate(passjson).encoding
    response = client.read
    puts response
    aaa = '{"page_name":"login","admin_pw":"aaa"}'
    my_hash = JSON.parse(aaa)
    #my_hash = JSON.parse(line, :quirks_mode => true)
    if my_hash[verify] == 'true'
      render 'welcome/index'
    else
      render 'login/index'
    end
    client.close
  end

end

