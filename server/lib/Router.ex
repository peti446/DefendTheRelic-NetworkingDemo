defmodule Router do
  use GenServer

##Documentation to use https://hexdocs.pm/elixir/1.4.1/

  @pName :s6073335_Router

  def start_link() do
    GenServer.start_link(__MODULE__, :ok, name: @pName)
  end

  def init(:ok) do
    {:ok, %{nextRegID: 0, prefixReg: Utility.random_string(6), players: %{}, games: %{}, lobbies: %{}, rsa: RSA.generate_keyPair()}}
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

  def server_user_disconected(name) do
    GenServer.cast(@pName, {:tcp_conn_closed, name})
  end

  #Handle messages functions

  ##Cast Handles
  def handle_cast({:tcp_conn_closed, name}, state) do
    IO.puts("Trying to destroy #{inspect name}")
    state = case Map.fetch(state.players, name) do
      {:ok, playerMap} ->
        case Utility.networkStringSplitter(playerMap.where_at) do
          ["GameLobby", id] ->
            {_,_, state} = handle_call({nil, [name, "5", id, "exit"]}, self(), state)
            state
          ["InGame", id] ->
            {_,_, state} = handle_call({nil, [name, "10", name]}, self(), state)
            state
          [_] ->
            state
        end
      :error->
        state
    end
    case Map.pop(state.players, name) do
       {nil, _} ->
        IO.puts("Could not destroy it")
        {:noreply, state}
      {_ , newmap} ->
        state = Map.put(state, :players, newmap)
        IO.puts "Destroyed!"
        {:noreply, state}
    end
  end

  def handle_cast(msg, state) do
    IO.puts("Could not handle cast message #{inspect msg}")
    {:noreply, state}
  end

  ##Internal helper functions
  def send_GameLobbyUpdate_to_all(id, players, lobbies)  do
    case Map.fetch(lobbies, id) do
      {:ok, lobby} ->
        Enum.each(players, fn {_,v} ->
          if(not String.starts_with?(v.where_at, "InGame")) do
            Router.send_GameLobbyUpdate_to_player(v, id, lobby)
          end
        end)
      :error ->
        IO.puts("There is no lobby with name #{inspect id} in the list")
    end
  end

  def send_GameLobbyUpdate_to_player(player, id, lobby) do
    lobbyMSG = Utility.add_header_to_str(id) <>
               Utility.add_header_to_str(lobby.t1_player1) <>
               Utility.add_header_to_str(lobby.t1_player2) <>
               Utility.add_header_to_str(lobby.t2_player1) <>
               Utility.add_header_to_str(lobby.t2_player2) <>
               Utility.add_header_to_str(to_string(lobby.current_status))
    TCP.send_tpc_encrypted_message(player.tcp_socket, 4, lobbyMSG, player.aesKey)
  end

  def send_all_GameLobbies_to_player(player, lobbies) do
    Enum.each(lobbies, fn {id, lobby} ->
      Router.send_GameLobbyUpdate_to_player(player, id, lobby)
    end)
  end

  def add_player_to_lobby(id, lobby, player) do
    whereAtTemp = "GameLobby-::-" <> id
    cond do
      lobby.t1_player1 == "NONE" ->
        lobby = Map.put(lobby, :t1_player1, player.display_name)
        player = Map.put(player, :where_at, whereAtTemp)
        {lobby, player}
      lobby.t1_player2 == "NONE" ->
        lobby = Map.put(lobby, :t1_player2, player.display_name)
        player = Map.put(player, :where_at, whereAtTemp)
        {lobby, player}
      lobby.t2_player1 == "NONE" ->
        lobby = Map.put(lobby, :t2_player1, player.display_name)
        player = Map.put(player, :where_at, whereAtTemp)
        {lobby, player}
      lobby.t2_player2 == "NONE" ->
        lobby = Map.put(lobby, :t2_player2, player.display_name)
        player = Map.put(player, :where_at, whereAtTemp)
        {lobby, player}
      true ->
        {lobby, player}
    end
  end

  def remove_player_fom_lobby(lobby, player) do
    whereAtTemp = "Lobby"
    cond do
      lobby.t1_player1 == player.display_name ->
        lobby = Map.put(lobby, :t1_player1, "NONE")
        player = Map.put(player, :where_at, whereAtTemp)
        {lobby, player}
      lobby.t1_player2 == player.display_name ->
        lobby = Map.put(lobby, :t1_player2, "NONE")
        player = Map.put(player, :where_at, whereAtTemp)
        {lobby, player}
      lobby.t2_player1 == player.display_name ->
        lobby = Map.put(lobby, :t2_player1, "NONE")
        player = Map.put(player, :where_at, whereAtTemp)
        {lobby, player}
      lobby.t2_player2 == player.display_name ->
        lobby = Map.put(lobby, :t2_player2, "NONE")
        player = Map.put(player, :where_at, whereAtTemp)
        {lobby, player}
      true ->
        {lobby, player}
    end
  end

  def is_lobby_empty(lobby) do
    lobby.t1_player1 == "NONE" and
    lobby.t1_player2 == "NONE" and
    lobby.t2_player1 == "NONE" and
    lobby.t2_player2 == "NONE"
  end

  def get_player_team(name, lobby) do
    cond do
      lobby.t1_player1 == name or lobby.t1_player2 == name ->
        :team_1
      lobby.t2_player1 == name or lobby.t2_player2 == name ->
        :team_2
      true ->
        :none
    end
  end

  ##Call handles
  def handle_call({socket, [userID, "1", encrpted]}, from, state) do
    case Map.fetch(state.players, userID) do
      {:ok, userMap} ->
        {:ok, aesCipher} = Base.decode64(encrpted, ignore: :whitespace)
        {:ok, aesKey} = Map.fetch(userMap, :aesKey)
        str = AES.decrypt(aesCipher, aesKey)
        Router.handle_call({socket,[userID | Utility.networkStringSplitter(str)]}, from, state)
      :error ->
        IO.puts("Recived encrypted message from user #{inspect userID} but the user is not registered ?");
        {:reply ,state, state}
    end
  end

  def handle_call({socket, address, port, [userID, "1", encrpted]}, from, state) do
    case Map.fetch(state.players, userID) do
      {:ok, userMap} ->
        {:ok, aesCipher} = Base.decode64(encrpted, ignore: :whitespace)
        {:ok, aesKey} = Map.fetch(userMap, :aesKey)
        str = AES.decrypt(aesCipher, aesKey)
        Router.handle_call({socket, address, port,[userID | Utility.networkStringSplitter(str)]}, from, state)
      :error ->
        IO.puts("Recived encrypted message from user #{inspect userID} but the user is not registered ?");
        {:reply ,state, state}
    end
  end

##---------------------------------------------------------------------------------------------------------------------------
##----------- Game Handle calls
def handle_call({socket, _address, _port, [userID, "7", who, posX, posY, dir, status]}, _from, state) do
  case Map.fetch(state.players, userID) do
    {:ok, cuserMap} ->
      case Utility.networkStringSplitter(cuserMap.where_at) do
        ["InGame", id] ->
          case Map.fetch(state.lobbies, id) do
            {:ok, lobby} ->
              returnMSG = Utility.add_header_to_str(who) <>
                          Utility.add_header_to_str(posX) <>
                          Utility.add_header_to_str(posY) <>
                          <<String.to_integer(dir)::unsigned-big-integer-16>> <>
                          <<String.to_integer(status)::unsigned-big-integer-16>>
              Enum.each(state.players, fn({uID, userMap}) ->
                if((userMap.display_name == lobby.t1_player1 or userMap.display_name == lobby.t1_player2
                or userMap.display_name == lobby.t2_player1 or userMap.display_name == lobby.t2_player2) and uID != userID) do
                  UDP.send_udp_encrypted_message(socket, userMap.udp_address, userMap.udp_port, 7, returnMSG, userMap.aesKey)
                  :ok
                end
                :ok
              end)
              {:reply ,state, state}
            :error ->
              {:reply ,state, state}
          end
        [_] ->
        {:reply ,state, state}
      end
    :error ->
      IO.puts("Recived udpate pos direction message from user #{inspect userID} but the user is not registered ?");
      {:reply ,state, state}
  end
end

def handle_call({socket, _address, _port, [userID, "8", who, posX, posY, dir, speed]}, _from, state) do
  case Map.fetch(state.players, userID) do
    {:ok, cuserMap} ->
      case Utility.networkStringSplitter(cuserMap.where_at) do
        ["InGame", id] ->
          case Map.fetch(state.lobbies, id) do
            {:ok, lobby} ->
              IO.puts("Spawning bullet at: x #{inspect posX} y #{inspect posY} dir #{inspect dir} speed #{inspect speed}")
              returnMSG = Utility.add_header_to_str(who) <>
                          Utility.add_header_to_str(posX) <>
                          Utility.add_header_to_str(posY) <>
                          <<String.to_integer(dir)::unsigned-big-integer-16>> <>
                          Utility.add_header_to_str(speed)
              Enum.each(state.players, fn({uID, userMap}) ->
                if((userMap.display_name == lobby.t1_player1 or userMap.display_name == lobby.t1_player2
                or userMap.display_name == lobby.t2_player1 or userMap.display_name == lobby.t2_player2) and uID != userID) do
                  UDP.send_udp_encrypted_message(socket, userMap.udp_address, userMap.udp_port, 8, returnMSG, userMap.aesKey)
                  :ok
                end
                :ok
              end)
              {:reply ,state, state}
            :error ->
              {:reply ,state, state}
          end
        [_] ->
        {:reply ,state, state}
      end
    :error ->
      IO.puts("Recived bullet instantiate message from user #{inspect userID} but the user is not registered ?");
      {:reply ,state, state}
  end
end


def handle_call({_socket, [userID, "9", who, killer]}, _from, state) do
  case Map.fetch(state.players, userID) do
    {:ok, cuserMap} ->
      case Utility.networkStringSplitter(cuserMap.where_at) do
        ["InGame", id] ->
          case Map.fetch(state.lobbies, id) do
            {:ok, lobby} ->
              returnMSG = Utility.add_header_to_str(who) <>
                          Utility.add_header_to_str(killer)
              Enum.each(state.players, fn({_uID, userMap}) ->
                if(userMap.display_name == lobby.t1_player1 or userMap.display_name == lobby.t1_player2
                or userMap.display_name == lobby.t2_player1 or userMap.display_name == lobby.t2_player2) do
                  TCP.send_tpc_encrypted_message(userMap.tcp_socket, 9, returnMSG, userMap.aesKey)
                  :ok
                end
                :ok
              end)
              newLobby = case Router.get_player_team(who, lobby) do
                :team_1 ->
                  Map.put(lobby, :team2_kills, lobby.team2_kills + 1)
                :team_2->
                  Map.put(lobby, :team1_kills, lobby.team1_kills + 1)
                _->
                  lobby
              end
              newLobbies = Map.put(state.lobbies, id, newLobby)
              state = Map.put(state, :lobbies, newLobbies)
              {:reply ,state, state}
            :error ->
              {:reply ,state, state}
          end
        [_] ->
        {:reply ,state, state}
      end
    :error ->
      IO.puts("Recived die message from user #{inspect userID} but the user is not registered ?");
      {:reply ,state, state}
  end
end

def handle_call({_socket, [userID, "10", whoDisconnected]}, _from, state) do
  case Map.fetch(state.players, userID) do
    {:ok, cuserMap} ->
      case Utility.networkStringSplitter(cuserMap.where_at) do
        ["InGame", id] ->
          case Map.fetch(state.lobbies, id) do
            {:ok, lobby} ->
              returnMSG = Utility.add_header_to_str(whoDisconnected)
              Enum.each(state.players, fn({_uID, userMap}) ->
                if((userMap.display_name == lobby.t1_player1 or userMap.display_name == lobby.t1_player2
                or userMap.display_name == lobby.t2_player1 or userMap.display_name == lobby.t2_player2) and userMap.display_name != whoDisconnected) do
                  TCP.send_tpc_encrypted_message(userMap.tcp_socket, 10, returnMSG, userMap.aesKey)
                  :ok
                end
                :ok
              end)
              {newLobby, playerMap} = Router.remove_player_fom_lobby(lobby, whoDisconnected)
              cuserMap = Map.put(state.players, userID, playerMap)
              state = Map.put(state, :players, cuserMap)
              newLobbies = Map.put(state.lobbies, id, newLobby)
              state = Map.put(state, :lobbies, newLobbies)
              ##Delete lobby if it is emptu
              if(Router.is_lobby_empty(newLobby)) do
                {_, newLobby} = Map.pop(state.lobbies, id)
                state = Map.put(state, :lobbies, newLobby)
                {:reply ,state, state}
              else
                {:reply ,state, state}
              end
            :error ->
              {:reply ,state, state}
          end
        [_] ->
        {:reply ,state, state}
      end
    :error ->
      IO.puts("Recived disconnect message from user #{inspect userID} but the user is not registered ?");
      {:reply ,state, state}
  end
end

def handle_call({_socket, [userID, "12", who, posX, posY]}, _from, state) do
  case Map.fetch(state.players, userID) do
    {:ok, cuserMap} ->
      case Utility.networkStringSplitter(cuserMap.where_at) do
        ["InGame", id] ->
          case Map.fetch(state.lobbies, id) do
            {:ok, lobby} ->
              returnMSG = Utility.add_header_to_str(who) <>
                          Utility.add_header_to_str(posX) <>
                          Utility.add_header_to_str(posY)
              IO.puts("Sending respawn #{inspect who} at #{inspect posX} amd #{inspect posY}")
              Enum.each(state.players, fn({_uID, userMap}) ->
                if(userMap.display_name == lobby.t1_player1 or userMap.display_name == lobby.t1_player2
                or userMap.display_name == lobby.t2_player1 or userMap.display_name == lobby.t2_player2) do
                  TCP.send_tpc_encrypted_message(userMap.tcp_socket, 12, returnMSG, userMap.aesKey)
                  :ok
                end
                :ok
              end)
              {:reply ,state, state}
            :error ->
              {:reply ,state, state}
          end
        [_] ->
        {:reply ,state, state}
      end
    :error ->
      IO.puts("Recived respawn message from user #{inspect userID} but the user is not registered ?");
      {:reply ,state, state}
  end
end

def handle_call({socket, _address, _port, [userID, "13", who, newAmmoAmount, newMaxAmmo]}, _from, state) do
  case Map.fetch(state.players, userID) do
    {:ok, cuserMap} ->
      case Utility.networkStringSplitter(cuserMap.where_at) do
        ["InGame", id] ->
          case Map.fetch(state.lobbies, id) do
            {:ok, lobby} ->
              returnMSG = Utility.add_header_to_str(who) <>
                          <<String.to_integer(newAmmoAmount)::big-integer-64>> <>
                          <<String.to_integer(newMaxAmmo)::big-integer-64>>
              Enum.each(state.players, fn({uID, userMap}) ->
                if((userMap.display_name == lobby.t1_player1 or userMap.display_name == lobby.t1_player2
                or userMap.display_name == lobby.t2_player1 or userMap.display_name == lobby.t2_player2) and uID != userID) do
                  UDP.send_udp_encrypted_message(socket, userMap.udp_address, userMap.udp_port, 13, returnMSG, userMap.aesKey)
                  :ok
                end
                :ok
              end)
              {:reply ,state, state}
            :error ->
              {:reply ,state, state}
          end
        [_] ->
        {:reply ,state, state}
      end
    :error ->
      IO.puts("Recived ammo update message from user #{inspect userID} but the user is not registered ?");
      {:reply ,state, state}
  end
end

def handle_call({socket, _address, _port, [userID, "14", who, newHP]}, _from, state) do
  case Map.fetch(state.players, userID) do
    {:ok, cuserMap} ->
      case Utility.networkStringSplitter(cuserMap.where_at) do
        ["InGame", id] ->
          case Map.fetch(state.lobbies, id) do
            {:ok, lobby} ->
              returnMSG = Utility.add_header_to_str(who) <>
                          <<String.to_integer(newHP)::big-integer-64>>
              Enum.each(state.players, fn({uID, userMap}) ->
                if((userMap.display_name == lobby.t1_player1 or userMap.display_name == lobby.t1_player2
                or userMap.display_name == lobby.t2_player1 or userMap.display_name == lobby.t2_player2) and uID != userID) do
                  UDP.send_udp_encrypted_message(socket, userMap.udp_address, userMap.udp_port, 14, returnMSG, userMap.aesKey)
                  :ok
                end
                :ok
              end)
              {:reply ,state, state}
            :error ->
              {:reply ,state, state}
          end
        [_] ->
        {:reply ,state, state}
      end
    :error ->
      IO.puts("Recived hp Update message from user #{inspect userID} but the user is not registered ?");
      {:reply ,state, state}
  end
end

##---------------------------------------------------------------------------------------------------------------------------

##---------------------------------------------------------------------------------------------------------------------------
##----------- Lobby Handle calls
  def handle_call({socket, [userID, "2", newName]}, _from, state) do
    case Map.fetch(state.players, userID) do
      {:ok, userMap} ->
        duplicated = Enum.any?(state.players, fn {_ , v} ->
          v.display_name == newName
        end)
        case duplicated do
          true ->
            TCP.send_tpc_message(socket, 0, Utility.add_header_to_str(""));
            {:reply ,state, state}
          false->
            {state, userMap} = case Utility.networkStringSplitter(userMap.where_at) do
              ["GameLobby", id] ->
                case Map.fetch(state.lobbies, id) do
                  {:ok, lobby} ->
                    newLobby = cond do
                      lobby.t1_player1 == userMap.display_name ->
                        Map.put(lobby, :t1_player1, newName)
                      lobby.t1_player2 == userMap.display_name ->
                        Map.put(lobby, :t1_player2, newName)
                      lobby.t2_player1 == userMap.display_name ->
                        Map.put(lobby, :t2_player1, newName)
                      lobby.t2_player2 == userMap.display_name ->
                        Map.put(lobby, :t2_player2, newName)
                      true->
                        lobby
                    end

                    ##Update the state lobby
                    newLobby = Map.put(state.lobbies, id, newLobby)
                    state = Map.put(state, :lobbies, newLobby)

                    ##Send Lobby to all
                    Router.send_GameLobbyUpdate_to_all(id, state.players, state.lobbies)
                    {state , userMap}
                  :error->
                    ##Update the player
                    userMap = Map.put(userMap, :where_at, "Lobby")
                    playerMap = Map.put(state.players, userID, userMap)
                    state = Map.put(state, :players, playerMap)
                    {state, userMap}
                end
              _ ->
                {state, userMap}
            end
            userMap = Map.put(userMap, :display_name, newName)
            playerMap = Map.put(state.players, userID, userMap)
            state = Map.put(state, :players, playerMap)
            TCP.send_tpc_encrypted_message(socket, 2, Utility.add_header_to_str(newName), userMap.aesKey);
            {:reply ,state, state}
        end
      :error ->
        IO.puts("Could not change the displayname to #{inspect newName} from #{inspect userID}. Player is not regisrted????? This should not be possible!! Maybe MIA??")
        {:reply ,state, state}
    end
  end

  def handle_call({_socket, [userID, "3", "GetAll"]}, _from, state) do
    case Map.fetch(state.players, userID) do
      {:ok, userMap} ->
        Router.send_all_GameLobbies_to_player(userMap, state.lobbies)
        {:reply ,state, state}
      :error ->
        IO.puts("Recived switch team in game lobby from user #{inspect userID} but the user is not registered ?");
        {:reply ,state, state}
    end
  end

  def handle_call({socket, [userID, "4", "-1create", t1_p1, t1_p2, t2_p1, t2_p2]}, from, state) do
    case Map.fetch(state.players, userID) do
      {:ok, userMap} ->
        ##Generate a lobby
        id = Utility.random_string(16)
        state = Kernel.put_in(state, [:lobbies, id], %{t1_player1: t1_p1, t1_player2: t1_p2, t2_player1: t2_p1, t2_player2: t2_p2, team1_kills: 0, team2_kills: 0, current_status: "waiting"})

        ##In case the player is in a game
        {:reply, state, state} = case Utility.networkStringSplitter(userMap.where_at) do
          ["GameLobby", idInGame] ->
            Router.handle_call({socket, [userID, "5", idInGame, "exit"]}, from, state)
           _ ->
            {:reply ,state, state}
        end

        ##Send Lobby to all
        Router.send_GameLobbyUpdate_to_all(id, state.players, state.lobbies)

        ##Update the player
        userMap = Map.put(userMap, :where_at, "GameLobby-::-" <> id)

        ##Update the player map with the new user values
        playerMap = Map.put(state.players, userID, userMap)
        state = Map.put(state, :players, playerMap)


        {:reply, state, state}
      :error->
        IO.puts("Recived create lobby message from user #{inspect userID} but the user is not registered ?");
        {:reply ,state, state}
    end
  end

  def handle_call({socket, [userID, "5", id, "join"]}, from, state) do
    case Map.fetch(state.players, userID) do
      {:ok, userMap} ->

        {:reply, state, state} = case Utility.networkStringSplitter(userMap.where_at) do
          ["GameLobby", idInGame] ->
            Router.handle_call({socket, [userID, "5", idInGame, "exit"]}, from, state)
           _ ->
            {:reply ,state, state}
        end

        ##Update the new lobby map
        state = case Map.fetch(state.lobbies, id) do
          {:ok, lobby} ->
            {newLobby, playerMap} = Router.add_player_to_lobby(id, lobby, userMap)
            ##Update the player map with the new user values
            playerMap = Map.put(state.players, userID, playerMap)
            state = Map.put(state, :players, playerMap)
            ##Update the state lobby
            newLobby = Map.put(state.lobbies, id, newLobby)
            Map.put(state, :lobbies, newLobby)
          :error ->
            state.lobbies
        end

        ##Send Lobby to all
        Router.send_GameLobbyUpdate_to_all(id, state.players, state.lobbies)
        {:reply ,state, state}
      :error ->
        IO.puts("Recived join a game lobby message from user #{inspect userID} but the user is not registered ?");
        {:reply ,state, state}
    end
  end

  def handle_call({_socket, [userID, "5", id, "exit"]}, _from, state) do
    case Map.fetch(state.players, userID) do
      {:ok, userMap} ->
        case Utility.networkStringSplitter(userMap.where_at) do
          [_, ^id] ->
            case Map.fetch(state.lobbies, id) do
              {:ok, lobby} ->
                {newLobby, newPlayer} = Router.remove_player_fom_lobby(lobby, userMap)
                  ##Update the player map with the new user values
                  playerMap = Map.put(state.players, userID, newPlayer)
                  state = Map.put(state, :players, playerMap)

                  ##Update the state lobby
                  newLobby = Map.put(state.lobbies, id, newLobby)
                  state = Map.put(state, :lobbies, newLobby)

                  ##Send Lobby to all
                  Router.send_GameLobbyUpdate_to_all(id, state.players, state.lobbies)

                  ##Delete lobby if it is emptu
                  if(Router.is_lobby_empty(newLobby[id])) do
                    {_, newLobby} = Map.pop(state.lobbies, id)
                    state = Map.put(state, :lobbies, newLobby)
                    {:reply ,state, state}
                  else
                    {:reply ,state, state}
                  end
              :error ->
                {:reply, state, state}
            end
           _ ->
            {:reply, state, state}
        end
      :error ->
        IO.puts("Recived join a game lobby message from user #{inspect userID} but the user is not registered ?");
        {:reply ,state, state}
    end
  end

  def handle_call({_socket, [userID, "5", id, "switch"]}, _from, state) do
    case Map.fetch(state.players, userID) do
      {:ok, userMap} ->
        case Map.fetch(state.lobbies, id) do
          {:ok, lobby} ->
            team = Router.get_player_team(userMap.display_name, lobby)
            {lobby, userMap} = Router.remove_player_fom_lobby(lobby, userMap)
            case team do
              :team_1 ->
                cond do
                  lobby.t2_player1 == "NONE" ->
                    state = Map.put(state, :lobbies, Map.put(state.lobbies, id, Map.put(lobby, :t2_player1, userMap.display_name)))
                    Router.send_GameLobbyUpdate_to_all(id, state.players, state.lobbies)
                    {:reply ,state, state}
                  lobby.t2_player2 == "NONE" ->
                    state = Map.put(state, :lobbies, Map.put(state.lobbies, id, Map.put(lobby, :t2_player2, userMap.display_name)))
                    Router.send_GameLobbyUpdate_to_all(id, state.players, state.lobbies)
                    {:reply ,state, state}
                  true->
                    {:reply ,state, state}
                end
              :team_2->
                cond do
                  lobby.t1_player1 == "NONE" ->
                    state = Map.put(state, :lobbies, Map.put(state.lobbies, id, Map.put(lobby, :t1_player1, userMap.display_name)))
                    Router.send_GameLobbyUpdate_to_all(id, state.players, state.lobbies)
                    {:reply ,state, state}
                  lobby.t1_player2 == "NONE" ->
                    state = Map.put(state, :lobbies, Map.put(state.lobbies, id, Map.put(lobby, :t1_player2, userMap.display_name)))
                    Router.send_GameLobbyUpdate_to_all(id, state.players, state.lobbies)
                    {:reply ,state, state}
                  true->
                    {:reply ,state, state}
                end
              :none ->
                {:reply ,state, state}
            end
          :error ->
            {:reply ,state, state}
        end
      :error ->
        IO.puts("Recived switch team in game lobby from user #{inspect userID} but the user is not registered ?");
        {:reply ,state, state}
    end
  end

  def handle_call({_socket, [_userID, "6", id, t1_p1, t1_p2, t2_p1, t2_p2]}, _from, state) do
    case Map.fetch(state.lobbies, id) do
      {:ok, lobby} ->
        lobby = Map.put(lobby, :current_status, "InGame")
        state = Map.put(state, :lobbies, Map.put(state.lobbies, id, lobby))
        lobbyMSG = Utility.add_header_to_str(id) <>
          Utility.add_header_to_str(lobby.t1_player1) <>
          Utility.add_header_to_str(lobby.t1_player2) <>
          Utility.add_header_to_str(lobby.t2_player1) <>
          Utility.add_header_to_str(lobby.t2_player2) <>
          Utility.add_header_to_str(to_string(lobby.current_status))

          newPlayerMap = Enum.map(state.players, fn({private, userMap}) ->
            if(userMap.display_name == t1_p1 or userMap.display_name == t1_p2
            or userMap.display_name == t2_p1 or userMap.display_name == t2_p2) do
              TCP.send_tpc_encrypted_message(userMap.tcp_socket, 6, lobbyMSG, userMap.aesKey)
              userMap = Map.put(userMap, :where_at, "InGame-::-" <> id)
              {private, userMap}
            else
              {private, userMap}
            end
          end) |> Enum.into(%{})
          Router.send_GameLobbyUpdate_to_all(id,state.players, state.lobbies)
          state = Map.put(state, :players, newPlayerMap)
        {:reply ,state, state}
      :error ->
        {:reply ,state, state}
    end
  end

##-------------------------------------------------------------------------------------------------------------------------------------------------
##Encryption setup, these does not follow any kind of packetges in the cpp file as they are expected in order and what data will arive

def handle_call({socket, ["st", "hsk"]}, _from, state) do
    {pubstr, _} = state.rsa
    TCP.send_tpc_message(socket, Utility.add_header_to_str(Base.encode64(pubstr)))
    {:reply ,state, state}
  end

  def handle_call({socket, address, port, ["notify", "me", "server"]}, _from, state) do
    UDP.send_udp_message(socket, address, port, <<TCP.port::big-16>> <> Utility.add_header_to_str(List.to_string(:inet.ntoa(address))))
    {:reply ,state, state}
  end

  def handle_call({socket, ["aesReg", encryptedKey]}, _from, state) do
      ##Get the aes key and IV
      {_, pri} = state.rsa;
      {:ok, rsaChipher} = Base.decode64(encryptedKey, ignore: :whitespace)
      [key, udpAddress, udpPort] = Utility.networkStringSplitter(RSA.decrypt(rsaChipher, pri))

      ##Get the current ID and increase it by on
      {currentID, newMap} = Map.get_and_update(state, :nextRegID, fn current_value -> {current_value, current_value+1} end)
      currentID = state.prefixReg <>  "_" <> to_string(currentID);
      state = newMap
      ##Create a player entri in the map
      displayName = Utility.random_string_32encode(8)
      {:ok, ip} = :inet.parse_address(Kernel.to_charlist(udpAddress))
      state = Kernel.put_in(state, [:players, currentID], %{tcp_socket: socket, udp_address: ip, udp_port: String.to_integer(udpPort), aesKey: key, display_name: displayName, where_at: "Lobby"})

      ##For debug porpuses
      IO.puts("Registered user: #{inspect currentID}")

      ##Send back the tcp response
      :gen_tcp.send(socket, Utility.add_header_to_str(Utility.add_header_to_str(Base.encode64(
                                                        AES.encrypt(
                                                          Utility.add_header_to_str("ok-::-" <>  currentID <> "-::-" <> displayName),
                                                          key
                                                        )
                                                       )
                                                      )
                  ))
      {:reply , {:user_rg, currentID}, state}
  end

##-------------------------------------------------------------------------------------------------------------------------------------------------

  def handle_call(msg, _from, state) do
    IO.puts("Could not handle call message #{inspect msg}")
    {:reply ,state, state}
  end
end
