defmodule TCP do
  def start_link() do
    port = 13000
    options = [:binary, reuseaddr: true, active: :once, backlog: 5]
    :gen_tcp.listen(port, options)
    {:ok, self()}
  end

  def accept_loop(socket) do
    case :gen_tcp.accept(listening_socket) do
      {:ok, socket} ->
        #TcpSupervisor.add_child()
      {:error, reason} ->

    end
  end

def tcp_client_loop(socket) do
 receive do
   {:tcp, socket, data} ->
     :inet.setopts(sock, [active: :once])
     tcp_client_loop(socket)
   {:tcp_closed, socket} ->
     ...
 end
end
