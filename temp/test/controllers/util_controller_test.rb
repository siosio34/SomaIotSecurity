require 'test_helper'

class UtilControllerTest < ActionController::TestCase
  test "should get setting" do
    get :setting
    assert_response :success
  end

  test "should get chlidren" do
    get :chlidren
    assert_response :success
  end

  test "should get otp" do
    get :otp
    assert_response :success
  end

  test "should get monitoring" do
    get :monitoring
    assert_response :success
  end

end
