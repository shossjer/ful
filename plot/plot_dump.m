## usage
##   start octave in this directory and execute
##   >> plot_dump('data_find_8_16_small.dump');
##   a PNG image will be created called 'out.png'
function plot_dump(filename)
  fid = fopen(filename, "rb");
  if (fid == -1)
    error("could not open file");
  endif

  style = fscanf(fid, "style %s\n");
  fscanf(fid, "xlabel ");
  xname = fgetl(fid);
  nresults = fscanf(fid, "results %u\n");
  results = struct("name", {}, "points", {}, "samples", {}, "xs", {}, "ys", {});

  for r = 1:nresults
    fscanf(fid, "name ");
    results(r).name = fgetl(fid);
    results(r).points = fscanf(fid, "points %u\n");
    results(r).samples = fscanf(fid, "samples %u\n");
    results(r).xs = zeros(1, results(r).points);
    results(r).ys = zeros(results(r).samples, results(r).points);
    for p = 1:results(r).points
      results(r).xs(1, p) = fscanf(fid, "x %u\ny");
      results(r).ys(:, p) = fscanf(fid, " %u", results(r).samples);
      fscanf(fid, "\n");
    endfor
  endfor

  fclose(fid);

  close all;

  xs = results(1).xs;
  ys = zeros(nresults, length(xs));
  for r = 1:nresults
    ys(r, :) = mean(results(r).ys);
  endfor

  if strcmp(style, 'plot')
    h = plot(xs, ys', '-o');
  elseif strcmp(style, 'loglog')
    h = loglog(xs, ys', '-o');
  else
    error("unknown style");
  endif

  legend(results(:).name, "location", "northwest");
  xlabel(xname);
  ylabel("nanoseconds");
  hold on;
  set(gca,'ColorOrderIndex',1);

  for r = 1:nresults
    ys(r, :) = min(results(r).ys);
  endfor

  if strcmp(style, 'plot')
    h = plot(xs, ys', ':');
  elseif strcmp(style, 'loglog')
    h = loglog(xs, ys', ':');
  else
    error("unknown style");
  endif

  hold off;

  print -dpng out.png;
endfunction
