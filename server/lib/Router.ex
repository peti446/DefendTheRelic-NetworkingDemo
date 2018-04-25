defmodule Router do
  use GenServer

  @pName :s6073335_Router

  def start_link() do
    GenServer.start_link(__MODULE__, :ok, name: @pName)
  end

  def init(:ok) do
    {:ok, %{nextRegID: 0, players: %{}, games: %{}, lobbies: %{}}}
  end

  def terminate(reason, state) do
    IO.puts "Router closed, reason #{inspect reason} with state #{inspect state}"
  end

  #API Calls
  def stop(reason \\ :normal, timeout \\ :infinity) do
    GenServer.stop(@pName, reason, timeout)
  end

  def server_handle_msg(msg) do
    GenServer.call(@pName, msg)
  end
  #Handle messages functions
  def handle_call({socket, address, port, ["notify", "me", "server"]}, _from, _state) do
    UDP.send_udp_msg(socket, address, port, TCP.port)
  end

  def handle_call({socket, ["reg", udpAddress, udpPort]}, _from, state) do
      ##Get the current ID and increase it by on
      {currentID,_} = Map.get_and_update(state, :nextRegID, fn current_value -> {current_value, current_value+1} end)

      ##Create a player entri in the map
      Kernel.put_in(state, [:players, currentID], [tcp_socker: socket, udp_address: udpAddress, udp_port: udpPort, display_name: ""]);
      ##Send back the tcp response
      TCP.send_tpc_message(socket, "ok-::-"+currentID);
  end

end
