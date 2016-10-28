require 'test_helper'

class WifisetsControllerTest < ActionController::TestCase
  setup do
    @wifiset = wifisets(:one)
  end

  test "should get index" do
    get :index
    assert_response :success
    assert_not_nil assigns(:wifisets)
  end

  test "should get new" do
    get :new
    assert_response :success
  end

  test "should create wifiset" do
    assert_difference('Wifiset.count') do
      post :create, wifiset: { pass: @wifiset.pass, ssid: @wifiset.ssid }
    end

    assert_redirected_to wifiset_path(assigns(:wifiset))
  end

  test "should show wifiset" do
    get :show, id: @wifiset
    assert_response :success
  end

  test "should get edit" do
    get :edit, id: @wifiset
    assert_response :success
  end

  test "should update wifiset" do
    patch :update, id: @wifiset, wifiset: { pass: @wifiset.pass, ssid: @wifiset.ssid }
    assert_redirected_to wifiset_path(assigns(:wifiset))
  end

  test "should destroy wifiset" do
    assert_difference('Wifiset.count', -1) do
      delete :destroy, id: @wifiset
    end

    assert_redirected_to wifisets_path
  end
end
