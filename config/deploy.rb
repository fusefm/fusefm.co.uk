load 'deploy' if respond_to?(:namespace) # cap2 differentiator
 
# Standard configuration
set :user, "www-data"  # user who owns the app
set :group, "www-data" # group who owns the app
set :password, "Fu53Apach3"
set :application, "fusefm.co.uk-test" # name of the project.  look at the 'deploy_to' section below for its usage...

#set :use_sudo, false  # I have not had to use this, but it may be needed in some setups...

# Only one of the following can be uncommented
#set :server_address, "YOUR_STAGING_MACHINE" # staging machines IP
set :server_address, "192.168.180.116" # production machines IP
 
# I like to deploy the code into /var/www/
# and then setup my webserver to point to it there.
set :deploy_to, "/mnt/webarray/pubwww/#{application}"
 
# SCM Stuff configure to taste, just remember the repository
set :repository,  "/mnt/webarray/git/repositories/drupal/.git"
set :scm, :git
set :branch, "production"
set :repository_cache, "git_production"
set :deploy_via, :remote_cache
set :scm_verbose,  true
set :ssh_options, { :forward_agent => true }
 
# Setup the server...
role :app, "#{server_address}", :primary => true
role :web, "#{server_address}", :primary => true
role :db, "#{server_address}", :primary => true
 
after 'deploy:setup', 'drupal:setup' # Here we setup the shared files directory
after 'deploy:symlink', 'drupal:symlink' # After symlinking the code we symlink the shared dirs
 
# Before restarting the webserver we fix all the 
# permissions and then symlink it to production
before 'deploy:restart', 'fusefm:permissions:fix', 'fusefm:symlink:application'
 
namespace :drupal do
  # shared directories
  task :setup, :except => { :no_release => true } do
    sudo "mkdir -p #{shared_path}/files"
    sudo "chmod -R g+w #{shared_path}/files"
    sudo "chown -R #{user}:#{group} #{deploy_to}"
    sudo "chown -R #{user}:#{group} #{shared_path}/files"
  end
 
  # symlink shared directories
  task :symlink, :except => { :no_release => true } do
    # this assumes that you have put your shared files in the "shared/files" directory
    # it also assumes that you have setup the repositories as I described in the blog post.
    
    sudo "ln -s ../../../../../shared/files #{latest_release}/drupal/sites/default/files" # assumes that 'files' is in drupal/sites/default directory (drupal 6)
  end
end
 
namespace :deploy do
  task :finalize_update, :except => { :no_release => true } do
    # I am currently not using this...
  end
 
  task :restart do
    # nothing to do here since we're on mod-php
  end
end
 
namespace :fusefm do
  namespace :symlink do
    task :application, :except => { :no_release => true } do
      # I am currenlty not using this...
    end
  end
 
  # change ownership
  namespace :permissions do
    task :fix, :except => { :no_release => true } do
      sudo "chmod -R 777 #{shared_path}/files"
    end
  end
 
end