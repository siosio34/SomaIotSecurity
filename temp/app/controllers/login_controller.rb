require 'socket'
require 'json'
require 'csv'

class LoginController < ApplicationController

  def index

  end

  def auth
    @pass = params[:password]
    passjson = {:page_name => 'login', :admin_pw => @pass}
    client = TCPSocket.new 'localhost', 9990
    client.puts JSON.generate(passjson)
    puts JSON.generate(passjson)
    puts JSON.generate(passjson).encoding
    response = client.recv(1024)
    puts response
    puts response.encoding
    changeEncodestr = response.force_encoding(Encoding::UTF_8)
    puts changeEncodestr
    puts changeEncodestr.encoding
    my_hash = JSON.parse(changeEncodestr)
    printf my_hash 	
    #my_hash = JSON.parse(line, :quirks_mode => true)
    if my_hash[verify] == 'true'
      render 'welcome/index'
    else
      render 'login/index'
    end
    client.close
  end

end

