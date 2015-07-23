function acc = kth_test_ProjectionRBF(path,scale_factor, shift, dim, p)


gamma = 1/dim;

PROJECTION_POLY_KERNEL = @(X,Y,gamma,p) ( gamma*( norm(X'*Y,'fro') )^2 )^p;

actions = importdata('actionNames.txt');
all_people = importdata('people_list.txt');


n_actions = size(actions,1);
n_peo =  size(all_people,1);
sc = 1;

acc = 0;
real_labels = zeros(n_peo*n_actions);
est_labels  = zeros(n_peo*n_actions);
  

load_sub_path =strcat(path, 'grass_points/kth-grass-point-one-dim', int2str(dim), '/sc', int2str(sc), '/scale', int2str(scale_factor), '-shift', int2str(shift) );

j=1;
  for pe_ts= 1: n_peo
      
      load_svm_model =strcat( './svm_models/projPoly_svm_run_', int2str(pe_ts), '_degree', num2str(p),'.mat');
      load(load_svm_model); %loading model and X_train

      for act_ts = 1:n_actions
          
          real_labels(j) = act_ts;
          name_load_gp = strcat( load_sub_path, '/grass_pt_', all_people(pe_ts), '_', actions(act_ts), '_dim', int2str(dim), '_p', num2str(p),  '.h5');     
          hinfo = hdf5info( char(name_load_gp) );
          one_video = hdf5read(hinfo.GroupHierarchy.Datasets(1));
          X_test(:,:,1) = one_video;
          
          K_test = compute_projPoly_kernel_svm(X_test,X_train, PROJECTION_POLY_KERNEL,gamma,p);
          [predict_label, accuracy, dec_values] = svmpredict([act_ts],[[1:size(K_test,1)]' K_test], model);
          est_labels(j) = predict_label;
          j=j+1;
          
          if predict_label == act_ts
              acc = acc+1;
          end

      end
      
     save_labels = strcat('./svm_results/projPoly_scale', int2str(scale_factor), '-shift', int2str(shift),'-degree',num2str(p),'.mat' );     
     save(save_labels, 'est_labels', 'real_labels');
  
  end
  %[acc n_peo n_actions]
  acc = acc*100/(n_peo*n_actions);
   
