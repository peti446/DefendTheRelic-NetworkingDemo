defmodule UDP do

  def start_link() do
    port = 13001
    options = [:binary, reuseaddr: true, active: false]
    case :gen_udp.open(port, options) do
        {:ok, socket} ->
          recv_loop(socket)
        {:error, reason} ->
          IO.puts("Could not start the UDP socket, reason #{inspect reason}")
          throw("UDP Opening Socket error")
    end
  end

  def recv_loop(socket) do
    case :gen_udp.recv(socket, 0) do
      {:ok, {address, port, message}}  ->
        
        recv_loop(socket)
      {:error, reason} ->
        IO.puts "Udp server failed:"
        IO.inspect reason.
        throw("UDP error trying to recive")
    end
  end

end
