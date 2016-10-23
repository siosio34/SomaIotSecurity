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
<<<<<<< HEAD
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
=======
    client.puts JSON.generate(passjson)

    while (line = client.gets)
      my_hash = JSON.parse(line)
      if my_hash[verify] == "true"
        render 'welcome/index'
      else
        render 'login/index'
      end
>>>>>>> 0d246dd2a271c83fa1ecfc5451a3445ff01d2aef
    end
    client.close
  end

<<<<<<< HEAD
=======

>>>>>>> 0d246dd2a271c83fa1ecfc5451a3445ff01d2aef
end

