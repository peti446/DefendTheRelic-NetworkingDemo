defmodule UDP do

  @udp_port 13001
  def port, do: @udp_port

  def start_link() do
    options = [:binary, reuseaddr: true, active: false]
    case :gen_udp.open(@udp_port, options) do
        {:ok, socket} ->
          pid = spawn(__MODULE__, :recv_loop, [socket])
          IO.puts("Started UDP over the port #{inspect @udp_port} with options #{inspect options}")
          {:ok, pid}
        {:error, reason} ->
          IO.puts("Could not start the UDP socket, reason #{inspect reason}")
          {:error, "Error starting UDP server (Opening)"}
    end
  end

  def recv_loop(socket) do
    case :gen_udp.recv(socket, 0) do
      {:ok, {address, port, message}}  ->
        messageList = Utility.networkStringSplitter(Utility.packetToString(message));
        Router.server_handle_msg({socket, address, port, messageList})
        recv_loop(socket)
      {:error, reason} ->
        IO.puts "Udp server failed:"
        IO.inspect reason
        :gen_udp.close(socket)
        throw("UDP error trying to recive")
    end
  end


## Server Api function calls
  def send_udp_msg(socket, address, port, msg) do
    :gen_udp.send(socket, address, port, msg)
  end

end
