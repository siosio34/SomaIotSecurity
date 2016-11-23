require 'json'

class WelcomeController < ApplicationController

  def index

  end



  def test
    my_hash = {:hello => "goodbye"}
    my_hash2 = {"page_name"=>"con_list","con_list"=>[{"1"=>"true","MAC":"c8:14:79:e8:3e:15","IP"=>"172.24.1.113","HOST_NAME"=>"android-ebff699db65b334b","rx"=>"96932","tx"=>"225657","connected"=>"1478777438","disconnected"=>"0"}, {"1"=>"true","MAC"=>"c8:14:79:e8:3e:15","IP"=>"172.24.1.113","HOST_NAME"=>"android-ebff699db65b334b","rx"=>"96932","tx"=>"225657","connected"=>"1478777438","disconnected"=>"0"}]}
    @test_json = my_hash.to_json.to_s.html_safe
    @test_json2 = my_hash2.to_json.to_s.html_safe
    puts @test_json
  end


end
