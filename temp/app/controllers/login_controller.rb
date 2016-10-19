require 'socket'
require 'json'

class LoginController < ApplicationController

  def index

  end

  def auth
    @pass = params[:password]
    passjson = {:page_name => 'login', :admin_pw => @pass}
    client = TCPSocket.new 'localhost', 9990
    client.puts JSON.generate(passjson)

    while (line = client.gets)
      puts line	
      my_hash = JSON.parse(line, :quirks_mode => true)
      if my_hash[verify] == 'true'
        render 'welcome/index'
      else
        render 'login/index'
      end
    end
    client.close
  end


end

